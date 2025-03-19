#include <iostream>
#include <string>
using namespace std;


class CPUJob {
    public:
        int job_id;                 // Unique identifier for the job
        int priority;               // Priority level of the job (1-10)
        int job_type;               // Job type (1-10)
        int cpu_time_consumed;      // Total CPU time consumed by the job
        int memory_consumed;        // Total memory consumed thus far

        CPUJob (int id, int prio, int type, int cpu_time, int mem);  // constructor to initialize a CPUJob object
        void display();             // Display job details

};

// Template class representing a node in the queue
template <class DT>
class Queue {
    public:
        DT* JobPointer;         // Pointer to a job (e.g., CPUJob)
        Queue<DT>* next;        // Pointer to the next node in the queue

        Queue(DT* job);         // Constructor to initialize a Queue not with a job

};


// NovelQueue class to manage CPUJobs in a queue-like structure
template <class DT> 
class NovelQueue {
    public:
        Queue<DT>* front;           // Pointer to the front of the queue
        Queue<DT>** NodePtrs;       // Array of pointers to Queue nodes (nodes in the queue)
        int size;                   // Number of elements in the queue    
        int capacity;               // CAPAVITY or Maximum number of elements the queue can hold

        NovelQueue();               // constructor
        ~NovelQueue();              // destructor

        // Method declaractions for queue operations
        void enqueue(DT* job);      // Add a job to the end of the queue
        DT* dequeue();              // Remove a job from the front of the queue
        void modify(int job_id, int new_priority, int new_job_type, int new_cpu_time_consumed, int new_memory_consumed);  // Modify a job in the queue
        void change(int job_id, int field_index, int new_value);    // Modify a job in the queue
        void promote(int job_id, int positions);                    // Promote a job in the queue
        // NovelQueue reorder(int attribute_index);                    // Reorder the queue based on a specific attribute
        // NovelQueue<DT>& reorder (int attribute_index);
        void reorder (int attribute_index);                           // Reorder the queue based on a specific attribute
        void display();             // Display all jobs in the queue
        int count();                // Return the number of jobs in the queue
        void listJobs();            // List all jobs in the queue
        void sortNodePtrs();        // Sort the array of node pointers based on a specific attribute
        void sortByPriority();       // Sort the queue based on priority
        void sortByJobType();
        void sortByCpuTimeConsumed();
        void sortByMemoryConsumed();


};



// Implementations for the CPUJob class methods

// Constructor for CPUJob
CPUJob::CPUJob(int id, int prio, int type, int cpu_time, int mem) {
    job_id = id;
    priority = prio,
    job_type = type,
    cpu_time_consumed = cpu_time;
    memory_consumed = mem;
}

// Method to display job details
void CPUJob:: display () {
    cout    << "Job ID: " << job_id
            <<", Priority: " << priority
            <<", Job Type: " << job_type
            <<", CPU Time Consumed: " << cpu_time_consumed
            <<", Memory Consumed: " << memory_consumed << endl;

}

// Implementation of the Queue class methods

// Constructor for Queue
template <class DT>
Queue<DT>::Queue(DT* job) {
    JobPointer = job;           // Initialize JobPointer with the job passed to the constructor
    next = nullptr;             // Initialize next pointer to nullptr
}

// Implementation of the NovelQueue methods

// Constructor for NovelQueue
template<class DT>
NovelQueue <DT> :: NovelQueue() {
    front  =  nullptr;                          // Initalize front pointer to nullptr
    size   =  0;                                // Initialize size to 0
    capacity =  20;                             // default capacity of 20
    NodePtrs = new Queue<DT>*[capacity];        // Initialize with the default capacity

}

// Method to add a job to the end of the queue
template <class DT>
NovelQueue<DT>::~NovelQueue() {
    for (int i = 0; i < size; i++) {
        delete NodePtrs[i]->JobPointer;     // clean up each CPUJobs
        delete NodePtrs[i];                 // clean up each node

    }

    delete[] NodePtrs;  // clean up dynamic array of node pointers

}

// method implementations
template <class DT>
void NovelQueue<DT>::enqueue(DT* newJob) {

     //cout << "trying to enqueue" << endl;
     // cout << "Job ID " << newJob->job_id <<endl;

    // Check for duplicate job ID
    for (int i = 0; i < size; ++i) {
        // cout << "checked if the job is already there " << endl;
        if (NodePtrs[i]->JobPointer->job_id == newJob->job_id) {
            cout << "Job ID " << newJob->job_id << " already exists!" << endl;
            delete NodePtrs[i]->JobPointer; // Free the old job memory
            NodePtrs[i]->JobPointer = new DT(*newJob); // Update with new job details
            return;     // Prevent duplicate entry
        
        }
    }

    if (size >= capacity) {
        cout  << "Queue is full!" << endl;

        // resize the NodePtrs array
        Queue<DT>** newPtrs = new Queue<DT>*[capacity * 2];
        for (int i = 0; i < capacity; ++i) {
            newPtrs[i] = NodePtrs[i];
        }
        delete[] NodePtrs;
        NodePtrs = newPtrs;
        capacity *= 2;

    }

    DT* jobCopy = new DT(*newJob);                  // Create a copy of the job
    Queue<DT>* newNode = new Queue<DT>(jobCopy);    // Create a new node with the job copy
    newNode -> next = nullptr;
    //cout << jobCopy;
    //cout << size;
    
    if (!front) {
        front = newNode;        // If the queue is empty, set the front to the new node
    }

    else {
            Queue<DT>* temp = front;
            while (temp->next) {
                temp = temp->next;
            }
        temp->next = newNode;   // Add the new node to the end of the queue
    }

    // Add to NodePtrs and resize if necessary

    NodePtrs[size] = newNode;
    size++;
    cout << "Enqueued Job: " << endl;
    jobCopy->display();
    cout << "Jobs after enqueue:" << endl;
    display();
}

// Method to remove a job from the front of the queue by deletion
template <class DT>
DT* NovelQueue<DT>::dequeue() {
    if (!front) {
        cout << "Queue is empty. Cannot dequeue." << endl;
        return nullptr; // Return nullptr if the queue is empty
    }

    Queue<DT>* temp = front;        // Store the front node
    DT* job = temp->JobPointer;     // Get the job pointer
    // Move the front pointer to the next node
    front = front->next;
    // Remove from NodePtrs
    for (int i = 0; i < size; ++i) {
        if (NodePtrs[i] == temp) {
            // Shift all elements to the left
            for (int j = i ; j < size - 1; ++j) {
                NodePtrs[j] = NodePtrs[j + 1];
            }
            NodePtrs[size - 1] = nullptr; // Clear the last pointer
            break;
        }
    }

    size--;                             // Decrease the size of the queue
    cout << "Dequeued Job: " << endl;
    job->display();                     // Display the job details
    cout << "Jobs after dequeue:" << endl;
    display();                          // Display all jobs in the queue

    delete temp;    // Clean up the dequeued node
    return job;     // Return the job pointer
}

// Method to modify a job in the queue
template <class DT>
void NovelQueue<DT>::modify (int job_id, int new_priority, int new_job_type, int new_cpu_time_consumed, int new_memory_consumed){
    for (int i = 0; i < size; ++i) {
        if (NodePtrs[i]->JobPointer->job_id == job_id) {
            NodePtrs[i]->JobPointer->priority = new_priority;
            NodePtrs[i]->JobPointer->job_type = new_job_type;
            NodePtrs[i]->JobPointer->cpu_time_consumed = new_cpu_time_consumed;
            NodePtrs[i]->JobPointer->memory_consumed = new_memory_consumed;
            cout << "Modified Job ID " << job_id << ":" << endl;
            NodePtrs[i]->JobPointer->display();
            return;
        }
    }
    // If job ID is not found, display an appropriate message
    cout << "Job ID " << job_id << " not found." << endl;
}

// Method to change a specific field of a job in the queue
template <class DT>
void NovelQueue<DT>::change (int job_id, int field_index, int new_value){
    for (int i = 0; i < size; ++i) {
        if (NodePtrs[i]->JobPointer->job_id == job_id) {
            switch (field_index) {
                case 1:
                    NodePtrs[i]->JobPointer->priority = new_value;
                    break;
                case 2:
                    NodePtrs[i]->JobPointer->job_type = new_value;
                    break;
                case 3:
                    NodePtrs[i]->JobPointer->cpu_time_consumed = new_value;
                    break;
                case 4:
                    NodePtrs[i]->JobPointer->memory_consumed = new_value;
                    break;
                default :
                    cout << "invalid field index." << endl;
                    return;
            }
            cout << "Changed Job ID "  << job_id << " field " << field_index << " to " << new_value << ":" << endl;
            NodePtrs[i]->JobPointer->display();
            cout << "Jobs after changing field: " << endl;
            display();
            return;
        }
    }
    cout << "Job with ID " << job_id << " not found in the queue." << endl;
}

// Method to promote a job in the queue
template <class DT>
void NovelQueue<DT>::promote(int job_id, int positions) {
    for (int i = 0; i < size; ++i) {
        if (NodePtrs[i]->JobPointer->job_id == job_id) {
            // Calculate the new position after promotion
            int new_pos = i - positions;
            if (new_pos < 0) {
                new_pos = 0; // Ensure it doesn't go out of bounds
            }

            // If the job is already at the new position, no need to promote
            if (new_pos == i) {
                cout << "Job ID " << job_id << " is already at the desired position." << endl;
                return;
            }

            // Store the job to be promoted
            Queue<DT>* temp = NodePtrs[i];

            // Shift jobs down to make space for the promoted job
            for (int j = i; j > new_pos; --j) {
                NodePtrs[j] = NodePtrs[j - 1];
            }

            // Place the promoted job in its new position
            NodePtrs[new_pos] = temp;

            if (new_pos > 0) {
                NodePtrs[new_pos -1 ]->next = NodePtrs[new_pos];
            }
            else {
                front = NodePtrs[new_pos];
            }

            NodePtrs[new_pos]-> next = (new_pos + 1 < size) ? NodePtrs[new_pos + 1] : nullptr;

            // Update the linked list to reflect the new order
/*            if (new_pos == 0) {
                // If promoted to the front, update the front pointer
                front = temp;
            } 
            
            else {
                // Update the next pointer of the previous node
                NodePtrs[new_pos - 1]->next = temp;
            }

            // Update the next pointer of the promoted job
            temp->next = (new_pos + 1 < size) ? NodePtrs[new_pos + 1] : nullptr;
*/
            cout << "Promoted Job ID " << job_id << " by " << positions << " Positions(s): " << endl;
            temp->JobPointer->display();
            cout << "Jobs after promotion: " << endl;
            display();
            return;
        }
    }
    cout << "Job with  ID " << job_id << " not found in the queue." << endl;
}

// Method to reorder the queue based on a specific attribute
template <class DT>
void NovelQueue<DT>::reorder(int attribute_index) {
switch (attribute_index) {
        case 1:   // Reorder by Job ID
            sortNodePtrs();
            cout << "Reordered Queue by attribute 1: " << endl;
            display();
            break;
        case 2:
            sortByPriority(); // Sort by priority
            cout << "Reordered Queue by attribute 2: " << endl;
            display();
            break;
        case 3:
            sortByJobType(); // Sort by job type
            cout << "Reordered Queue by attribute 3: " << endl;
            display();
            break;
        case 4:
            sortByCpuTimeConsumed(); // Sort by CPU time consumed
            cout << "Reordered Queue by attribute 4: " << endl;
            display();
            break;
        case 5:
            sortByMemoryConsumed(); // Sort by memory consumed
            cout << "Reordered Queue by attribute 5: " << endl;
            display();
            break;
        default:
            cout << "Invalid attribute index. Please use 1 for job type, 2 for priority, 3 for CPU time consumed, or 4 for memory consumed." << endl;
            return; // Exit if the attribute index is invalid
    }
}

// Method to display all jobs in the queue
template <class DT>
void NovelQueue<DT>::display (){
    for (int i = 0; i < size; ++i) {
        if (NodePtrs[i]->JobPointer !=  nullptr) {
            NodePtrs[i]->JobPointer->display();
        } else {
            cout << "Invalid job pointer." << endl;
        }
    }
    
}

// Method to return the number of jobs in the queue 
template <class DT>
int NovelQueue<DT>::count () {
    return size;
}

// Methods to list all jobs in the queue 
template <class DT>
void NovelQueue<DT>::listJobs () {
    cout << "List of jobs sorted by job IDs:" << endl;
    sortNodePtrs();
    for (int i = 0; i < size; ++i) {
        NodePtrs[i]->JobPointer->display();
    }
}




// Method to sort the array of node pointers based on a the job_id 
template <class DT>
void NovelQueue<DT>::sortNodePtrs () {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (NodePtrs[j]->JobPointer->job_id > NodePtrs[j + 1]->JobPointer->job_id) {
                Queue<DT>* temp = NodePtrs[j];
                NodePtrs[j] = NodePtrs[j + 1];
                NodePtrs[j + 1] = temp;
            }
        }
    }
}

template <class DT>
void NovelQueue<DT>::sortByPriority() {
    // Sort the NodePtrs array based on the priority attribute
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            // Compare the priorities of the jobs
            if (NodePtrs[j]->JobPointer->priority > NodePtrs[j + 1]->JobPointer->priority) {
                // Swap the pointers in the NodePtrs array
                Queue<DT>* temp = NodePtrs[j];
                NodePtrs[j] = NodePtrs[j + 1];
                NodePtrs[j + 1] = temp;
            }
        }
    }

    // Update the front pointer to reflect the new order
    front = NodePtrs[0]; // The new front will be the first job in the sorted array
    for (int i = 0; i < size - 1; ++i) {
        NodePtrs[i]->next = NodePtrs[i + 1]; // Link nodes together
    }
    NodePtrs[size - 1]->next = nullptr; // Last node points to nullptr

}

template <class DT>
void NovelQueue<DT>::sortByJobType() {
    // Sort the NodePtrs array based on the job_type attribute
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            // Compare the job types of the jobs
            if (NodePtrs[j]->JobPointer->job_type > NodePtrs[j + 1]->JobPointer->job_type) {
                // Swap the pointers in the NodePtrs array
                Queue<DT>* temp = NodePtrs[j];
                NodePtrs[j] = NodePtrs[j + 1];
                NodePtrs[j + 1] = temp;
            }
        }
    }

    // Update the front pointer to reflect the new order
    front = NodePtrs[0]; // The new front will be the first job in the sorted array
    for (int i = 0; i < size - 1; ++i) {
        NodePtrs[i]->next = NodePtrs[i + 1]; // Link nodes together
    }
    NodePtrs[size - 1]->next = nullptr; // Last node points to nullptr

}

template <class DT>
void NovelQueue<DT>::sortByCpuTimeConsumed() {
    // Sort the NodePtrs array based on the cpu_time_consumed attribute
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            // Compare the CPU times of the jobs
            if (NodePtrs[j]->JobPointer->cpu_time_consumed > NodePtrs[j + 1]->JobPointer->cpu_time_consumed) {
                // Swap the pointers in the NodePtrs array
                Queue<DT>* temp = NodePtrs[j];
                NodePtrs[j] = NodePtrs[j + 1];
                NodePtrs[j + 1] = temp;
            }
        }
    }

    // Update the front pointer to reflect the new order
    front = NodePtrs[0]; // The new front will be the first job in the sorted array
    for (int i = 0; i < size - 1; ++i) {
        NodePtrs[i]->next = NodePtrs[i + 1]; // Link nodes together
    }
    NodePtrs[size - 1]->next = nullptr; // Last node points to nullptr

}

template <class DT>
void NovelQueue<DT>::sortByMemoryConsumed() {
    // Sort the NodePtrs array based on the memory_consumed attribute
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            // Compare the memory consumed by the jobs
            if (NodePtrs[j]->JobPointer->memory_consumed > NodePtrs[j + 1]->JobPointer->memory_consumed) {
                // Swap the pointers in the NodePtrs array
                Queue<DT>* temp = NodePtrs[j];
                NodePtrs[j] = NodePtrs[j + 1];
                NodePtrs[j + 1] = temp;
            }
        }
    }

    // Update the front pointer to reflect the new order
    front = NodePtrs[0]; // The new front will be the first job in the sorted array
    for (int i = 0; i < size - 1; ++i) {
        NodePtrs[i]->next = NodePtrs[i + 1]; // Link nodes together
    }
    NodePtrs[size - 1]->next = nullptr; // Last node points to nullptr

}

int main() {
    int n;  // numnber of commands 
    cin >> n;  // read the number of commands

    // Instatiate a NovelQueue for CPUJob pointers
    NovelQueue<CPUJob> myNovelQueue;

    char command;    // Variable to store the commmand type

    // VARIABLES 
    // Variable for Job attributes
    int job_id, priority, job_type, cpu_time_consumed, memory_consumed;
    // Variables for modifying a job
    int new_priority, new_job_type, new_cpu_time_consumed;
    int new_memory_consumed;
    int field_index, new_value;
    // Variable for the number of positions in the 'Promote' command
    int positions;
    // Variable for the 'Reorder' command
    int attribute_index;
    
    // Read each command Process

    for (int i = 0; i < n; ++i) {
        cin >> command;   // Read the command type
        // cout << "Reading command: " << command << endl; // Debug statement

        switch (command) {
            case 'A' : { // Add (Enqueue)
                cin >> job_id >> priority >> job_type;
                cin >> cpu_time_consumed >> memory_consumed;
                CPUJob*  newJob = new CPUJob(job_id, priority, job_type, cpu_time_consumed, memory_consumed);
                myNovelQueue.enqueue(newJob);
                break;
            }

            case 'R': { // Remove (Dequeue)
                myNovelQueue.dequeue(); // Directly call dequeue, it handles display
                break;
            }

            case 'C': {   // Change Job Values
                cin >> job_id >> field_index >> new_value;
                // (*myNovelQueue).change (job_id, field_index, new_value);
                myNovelQueue.change(job_id, field_index, new_value);
                break;
            }

            case 'M': {   // Modify
                cin >> job_id >> new_priority >> new_job_type;
                cin >> new_cpu_time_consumed >> new_memory_consumed;
                myNovelQueue.modify(job_id, new_priority, new_job_type, new_cpu_time_consumed, new_memory_consumed);
                cout << "Jobs after modification: " << endl;
                myNovelQueue.display();
                break;
            }

            case 'P':{   // Promote
                cin >> job_id >> positions;
                myNovelQueue.promote(job_id, positions);
                break;
            }

            case 'O':{   // Reorder
                cin >> attribute_index;
                // NovelQueue<CPUJob> reorderedQueue = myNovelQueue.reorder(attribute_index);
                myNovelQueue.reorder(attribute_index);
                // cout << "Reordered Queue by attribute " << attribute_index << ":" << endl;
                // reorderedQueue.display();
                break;
            }

            case 'D': {  // Display
                cout <<  "Displaying all jobs in the queue: " << endl;
                myNovelQueue.display();
                break;
            }

            case 'N': {    // Count
                cout << "Number of elements in the queue: "  << myNovelQueue.count() << endl;
                break;
            }

            case 'L': {    // List Jobs
                myNovelQueue.listJobs();
                break;
            }

            default:
                cout << "Invalid Command" << endl;
                break;
        }
    }

    return 0;
}



/*
The program implements a queue management system for CPU jobs using a linked list structure.
It defines a CPUJob class to encapsulate job attributes such as job ID, priority, job type, CPU time consumed, and memory consumed.
The NovelQueue class manages these jobs through various methods:
Enqueue: Adds a new job to the end of the queue while checking for duplicates.
Dequeue: Removes the job at the front of the queue and returns it.
Modify: Updates multiple attributes of a specified job.
Change: Alters a specific attribute of a job based on an index.
Promote: Moves a job up in the queue based on specified positions.
Reorder: Reorders the queue based on a specified attribute.
Display: Shows all jobs in the queue.
Count: Returns the number of jobs currently in the queue.
The program processes user commands to manipulate the queue, allowing for dynamic job management and real-time updates.
each time a command is passed and executted the queue is updated accordingly and is displayed.



Questions asked to AI tool to help debug and fix errors while writing the project:
1. How can I check if the CPUJob class is correctly initialized before adding it to the NovelQueue?
2. What should I do if the program crashes when trying to dequeue a job from an empty queue?
3. How can I ensure that the memory allocated for CPUJob objects is properly released when they are removed from the queue?
4. What debugging steps should I take if I encounter a segmentation fault when modifying job attributes?
5. How can I verify that the NovelQueue class correctly handles duplicate job entries?
6. What is the best way to implement error handling for invalid user inputs in the command-line interface?
7. How can I confirm that the linked list in NovelQueue is properly maintaining the correct order of jobs?
8. What tools can I use to check for memory leaks in my implementation of the NovelQueue?
9. How can I ensure that the destructor of NovelQueue is correctly freeing all allocated memory?
10. What should I do if the program fails to promote a job within the queue as expected?
11. How can I test if the enqueue function is correctly adding jobs to the queue?
12. What are the signs of a dangling pointer in my implementation, and how can I prevent them?
13. How can I implement a copy constructor for CPUJob to avoid shallow copies?
14. What debugging techniques can I use to trace the flow of job promotions in the queue?
15. How can I check that the NovelQueue is not allowing the addition of null CPUJob pointers?
16. What steps should I take if I notice that the queue size is not updating correctly after operations?
17. How can I use assertions to validate the state of the queue during enqueueing and dequeueing?
18. What are the implications of using raw pointers for managing CPUJob objects in this code?
19. How can I implement smart pointers to improve memory management in the NovelQueue?
20. What should I do if the program is consuming excessive memory during execution?
21. How can I log the state of the queue after each operation for better debugging?
22. How can I ensure that the CPUJob class correctly handles its attributes when modified?
23. What strategies can I use to optimize the performance of the enqueue and dequeue operations?
24. How can I verify that the command-line interface correctly reflects the current state of the queue?
25. What debugging tools are available to visualize the linked list structure of the NovelQueue?

As I was writing the program and trying to undestand the concept of queue management system,
I found that asking targeted questions significantly enhances my understanding of the program,
data structures, and memory management. For instance, when I inquire about how to handle memory leaks
or ensure proper initialization of CPUJob objects, I not only learn about debugging techniques but also 
grasp the importance of efficient memory allocation. Exploring the functionality of the queue through
questions about enqueueing and dequeueing operations allows me to visualize how data flows within the structure.
Additionally, by considering the implications of using raw pointers versus smart pointers,
I become more aware of best practices in memory management. Overall, this process of questioning fosters
my critical thinking and problem-solving skills, equipping me with the knowledge necessary to tackle
complex programming challenges confidently.


the code does take in all the inputs and provide the expected outputs as per the project description.
However for input2-6.txt input file the code does takes in the command from the input file A 111 3 4 180 1500.
but does not enqueue the job for some reasons. I am not sure why this is happening.
I can see that the job inputs for the other inputs are being enqueued except for this one.


*/