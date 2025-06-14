#include <stdio.h>

#define MAX_PROCESSES 100

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;
} Process;

int main() {
    int n, time_quantum;
    Process p[MAX_PROCESSES];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter Time Quantum: ");
    scanf("%d", &time_quantum);

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter Arrival Time of P%d: ", p[i].pid);
        scanf("%d", &p[i].arrival_time);
        printf("Enter Burst Time of P%d: ", p[i].pid);
        scanf("%d", &p[i].burst_time);
        p[i].remaining_time = p[i].burst_time;
    }

    int time = 0, done = 0;
    int queue[MAX_PROCESSES], front = 0, rear = 0;
    int visited[MAX_PROCESSES] = {0};

    // Enqueue first arrived processes
    for (int i = 0; i < n; i++) {
        if (p[i].arrival_time == 0) {
            queue[rear++] = i;
            visited[i] = 1;
        }
    }

    printf("\nGantt Chart:\n");

    while (done < n) {
        if (front == rear) {
            // No process is ready, skip time
            time++;
            for (int i = 0; i < n; i++) {
                if (!visited[i] && p[i].arrival_time <= time) {
                    queue[rear++] = i;
                    visited[i] = 1;
                }
            }
            continue;
        }

        int i = queue[front++];
        printf("| P%d ", p[i].pid);

        if (p[i].remaining_time <= time_quantum) {
            time += p[i].remaining_time;
            p[i].remaining_time = 0;
            p[i].completion_time = time;
            p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
            p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
            done++;
        } else {
            time += time_quantum;
            p[i].remaining_time -= time_quantum;

            // Enqueue new arrivals during this time
            for (int j = 0; j < n; j++) {
                if (!visited[j] && p[j].arrival_time <= time) {
                    queue[rear++] = j;
                    visited[j] = 1;
                }
            }

            // Re-enqueue current process
            queue[rear++] = i;
        }

        // Enqueue any that just arrived (if not already in queue)
        for (int j = 0; j < n; j++) {
            if (!visited[j] && p[j].arrival_time <= time) {
                queue[rear++] = j;
                visited[j] = 1;
            }
        }
    }

    printf("|\n\n");

    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].arrival_time, p[i].burst_time,
               p[i].completion_time, p[i].turnaround_time, p[i].waiting_time);
    }

    return 0;
}
