#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <asm/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <unistd.h>

int main(int argc, char**argv) {
    int pid = fork();
    if(pid < 0) {
        perror("fork");
        exit(1);
    }else if(pid == 0) {
        int err = ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        if(err < 0) {
            perror("ptrace");
            exit(1);
        }

        kill(SIGSTOP, getpid());

        printf("hello\n");

        exit(420);

    }else {
        waitpid(pid, NULL, 0);

        while(1) {
            ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
            waitpid(pid, NULL, 0);

            struct user_regs_struct regs;
            ptrace(PTRACE_GETREGS, pid, NULL, &regs);

            printf("%lu\n", regs.a7);

            regs.a7 = -1;
            ptrace(PTRACE_SETREGS, pid, NULL, &regs);

            ptrace(PTRACE_SYSCALL, pid, 0, 0);
            waitpid(pid, 0, 0);
        }
    }
}