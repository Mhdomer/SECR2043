#include <stdio.h>

#define MAX 100

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int started;
} Process;

void round_robin(Process p[], int n, int tq, int *current_time) {
    int queue[MAX], front = 0, rear = 0;
    int done = 0, visited[MAX] = {0};

    for (int i = 0; i < n; i++) {
        if (p[i].arrival_time <= *current_time && !visited[i]) {
            queue[rear++] = i;
            visited[i] = 1;
        }
    }

    printf("\n[High Priority Queue – Round Robin Gantt Chart]\n");

    while (done < n) {
        if (front == rear) {
            (*current_time)++;
            for (int i = 0; i < n; i++) {
                if (!visited[i] && p[i].arrival_time <= *current_time) {
                    queue[rear++] = i;
                    visited[i] = 1;
                }
            }
            continue;
        }

        int i = queue[front++];
        printf("| P%d ", p[i].pid);

        if (p[i].remaining_time <= tq) {
            *current_time += p[i].remaining_time;
            p[i].remaining_time = 0;
            p[i].completion_time = *current_time;
            p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
            p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
            done++;
        } else {
            *current_time += tq;
            p[i].remaining_time -= tq;

            for (int j = 0; j < n; j++) {
                if (!visited[j] && p[j].arrival_time <= *current_time) {
                    queue[rear++] = j;
                    visited[j] = 1;
                }
            }

            queue[rear++] = i;
        }
    }
    printf("|\n");
}

void fcfs(Process p[], int n, int *current_time) {
    printf("\n[Low Priority Queue – FCFS Gantt Chart]\n");
    for (int i = 0; i < n; i++) {
        if (*current_time < p[i].arrival_time)
            *current_time = p[i].arrival_time;

        printf("| P%d ", p[i].pid);
        *current_time += p[i].burst_time;
        p[i].completion_time = *current_time;
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
    }
    printf("|\n");
}

int main() {
    int n, tq;
    Process high[MAX], low[MAX];
    int high_n = 0, low_n = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter Time Quantum for High Priority Queue: ");
    scanf("%d", &tq);

    for (int i = 0; i < n; i++) {
        Process temp;
        temp.pid = i + 1;
        temp.started = 0;
        printf("\nProcess %d\n", temp.pid);
        printf("Arrival Time: ");
        scanf("%d", &temp.arrival_time);
        printf("Burst Time: ");
        scanf("%d", &temp.burst_time);
        printf("Priority (1 = high, 10 = low): ");
        scanf("%d", &temp.priority);
        temp.remaining_time = temp.burst_time;

        if (temp.priority <= 3) {
            high[high_n++] = temp;
        } else {
            low[low_n++] = temp;
        }
    }

    // Sort both queues by arrival time (simple bubble sort for student level)
    for (int i = 0; i < high_n - 1; i++) {
        for (int j = 0; j < high_n - i - 1; j++) {
            if (high[j].arrival_time > high[j + 1].arrival_time) {
                Process temp = high[j];
                high[j] = high[j + 1];
                high[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < low_n - 1; i++) {
        for (int j = 0; j < low_n - i - 1; j++) {
            if (low[j].arrival_time > low[j + 1].arrival_time) {
                Process temp = low[j];
                low[j] = low[j + 1];
                low[j + 1] = temp;
            }
        }
    }

    int time = 0;
    if (high_n > 0)
        round_robin(high, high_n, tq, &time);
    if (low_n > 0)
        fcfs(low, low_n, &time);

    printf("\nFinal Results:\n");
    printf("PID\tAT\tBT\tPR\tCT\tTAT\tWT\n");

    for (int i = 0; i < high_n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n", high[i].pid, high[i].arrival_time,
               high[i].burst_time, high[i].priority, high[i].completion_time,
               high[i].turnaround_time, high[i].waiting_time);
    }

    for (int i = 0; i < low_n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n", low[i].pid, low[i].arrival_time,
               low[i].burst_time, low[i].priority, low[i].completion_time,
               low[i].turnaround_time, low[i].waiting_time);
    }

    return 0;
}
