#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <errno.h>
#include <syslog.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <linux/limits.h>

#include "help.h"
#include "config.h"
#include "file.h"
#include "sync.h"
#include "filelist.h"
#include "parse.h"

bool SYNC_IN_PROGRESS = false;
bool TIME_TO_DIE = false;

void forking();
void signal_force_synch();
void signal_kill();
void signal_stop();
void signal_connect();
int lock(bool lock);
int main(int argc, char* argv[])
{
    int pid = lock(false);
    signal_connect();

    if(argc == 2 &&argv[1][0] == '-' && argv[1][1] == 'h'){
        help(true);
        return EXIT_FAILURE;
    }
    if(pid == -1){
        printf("Error: cannot load file demon\n");
        return EXIT_FAILURE;
    }
    if(pid != 0){
        if(argc != 2 || argv[1][0] != '-'){
            printf("Demon is already running\n");
            help_launch();
            return EXIT_FAILURE;
        }else{
            switch (argv[1][1])
            {
            case 'F':
                printf("Sending SIGUSR1\n");
                kill(pid, SIGUSR1);
                return EXIT_SUCCESS;
                break;
            case 'S':
                printf("Sending SIGUSR2\n");
                kill(pid,SIGUSR2);
                return EXIT_SUCCESS;
                break;
            case 'K':
                printf("Sending SIGTTERM\n");
                kill(pid,SIGTERM);
                return EXIT_SUCCESS;
                break;
            
            default:
                printf("Unidentified argument");
                return EXIT_FAILURE;
                break;
            }
        }
    }

    config c = parse_args(argc,argv);

    if(!c.is_valid){
        printf("Error: invalid syntax\n");
        help(false);
        return EXIT_FAILURE;
    }

    if(!(check_dir(c.source_directory)) && !(check_dir(c.destination_directory))){
        printf("Error: either directory does not exist. Try again with valid directories\n");
        return EXIT_FAILURE;
    }

    char bufa[PATH_MAX+1],bufb[PATH_MAX+1];
    c.source_directory = realpath(c.source_directory, bufa);
    c.destination_directory = realpath(c.destination_directory,bufb);

    if(contains(c.source_directory, c.destination_directory)){
        printf("Error: dir cannot contain another dir\n");
        return EXIT_FAILURE;
    }

    openlog("demon_logs", LOG_PID | LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "Program start");
    forking();

    if(lock(true) != 0){
        syslog(LOG_CRIT, "Error: couldnt block file");
        signal_kill();
    }

    while(true){
        syslog(LOG_INFO, "I'm starting synchronisation");
        SYNC_IN_PROGRESS = true;
        sync_all(c);
        SYNC_IN_PROGRESS = false;
        if(TIME_TO_DIE){
            signal_kill();
        }
        syslog(LOG_INFO," Synchronisation completed, demon goes to sleep for %d seconds", c.sleep_time);
        sleep(c.sleep_time);
    }
    closelog();

return 0;
}

void signal_kill(){
    syslog(LOG_INFO, "Termination demon work");
    exit(EXIT_SUCCESS);
}
void signal_force_synch(){
    if(SYNC_IN_PROGRESS){
        syslog(LOG_INFO,"Forcing during syncing, process continues");
    }else{
        syslog(LOG_INFO,"Forced synchronisation, awakening of process");
    }
}
void signal_stop(){
    syslog(LOG_INFO,"Safe termination of work, waiting for syncing to finish");
    if(SYNC_IN_PROGRESS){
        TIME_TO_DIE=true;
    }else{
        signal_kill();
    }
}
void signal_connect(){
    signal(SIGUSR1, signal_force_synch);
    signal(SIGUSR2,signal_stop);
    signal(SIGTERM,signal_kill);
}
void forking(){
    syslog(LOG_INFO," Forking process");

    pid_t pid, sid;
    pid = fork();

    if(pid < 0){
        syslog(LOG_CRIT, "Error: couldn't separate process");
        exit(EXIT_FAILURE);
    }
    if(pid > 0){
        exit(EXIT_SUCCESS);
    }
    
    umask(0); //set mask of files

    sid =setsid();

    if(sid < 0){
        syslog(LOG_CRIT,"Error: couldn't create SID for child process");
        exit(EXIT_FAILURE);
    }
    //set directory
    if((chdir("/")) < 0){
        syslog(LOG_CRIT,"Error: couldn't change current directory");
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

int lock(bool lock){
    int lock_file = open("/tmp/demon.lock", O_CREAT | O_RDWR, 0666);
    struct flock lockp;
    fcntl(lock_file, F_GETLK, &lockp);
    if(lockp.l_type == 2){
        lockp.l_type = lock;
        if(fcntl(lock_file, F_SETLKW, &lockp) != -1){
            syslog(LOG_INFO, "File was succesfully blocked: /tmp/demon.lock");
            return 0; //block succes
        }else{
            return -1;//block failed
        }
    }else{
        return lockp.l_pid; //return pid of process whick locked
    }
}