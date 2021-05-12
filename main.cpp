
/*main.cpp*/
//
// Author: Adithya Jose, UIC, Spring 2021
// Assignment: Project 06 Part 1
//
// An intro to ClassReg class, and project 06.
// Builds upon the concepts learned in project 5 to implement a simple class registration system for CS classes.
//
#include <iostream>
#include <string>
#include <cmath>    
#include <iomanip>   
#include <fstream>
#include "classreg.h"

using namespace std;


//
// inputClassInfo
//
// Inputs the inputted file's data to a ClassReg array of 5 which should cover all the CS classes listed in the project.
// Detects if input file is invalid as well.
//
ClassReg* inputClassInfo(ClassReg* classes, int& x)
{
    string filename;
    cin >> filename;
    
    ifstream infile;        // declares an input file object
     
    infile.open(filename);        // opens input file
    
    // mitigation to make sure input file is legitmate otherwise prints error message
    
    if (!infile.good())  // file is NOT good, i.e. could not be opened:
    {
        cout << "**Error: unable to open input file '" << filename << "'" << endl;
        x = -1;
        return nullptr;
        
    }
    
    string courseName, netID;
    int capacity, priority;
    
    for (int i = 0; i < 5; ++i) // filling in array with data from file based on N
    {
        infile >> courseName;
        infile >> capacity;
        
        classes[i].setName(courseName);
        classes[i].setCapacity(capacity);
        
        infile >> netID;
        
        while (netID != "#")
        {
            classes[i].enrollStudent(netID);
            infile >> netID;
        }
        
        infile >> netID;
        
        while (netID != "#")
        {
            infile >> priority;
            classes[i].waitlistStudent(netID, priority);
            infile >> netID;
        }
        
    }
    
    infile.close();
    return classes;
}

//
// stats
//
// The “stats” command is triggered by “s” or “stats”. In response, the program outputs the number of
// students currently enrolled and waitlisted in each class.
//
void stats (ClassReg* classes)
{
    string courseName;
    
    for (int i = 0; i < 5; ++i) // filling in array with data from file based on N
    {
        courseName = classes[i].getName();
        
        cout << courseName << ": enrolled=" << classes[i].numEnrolled() << ", waitlisted=" << classes[i].numWaitlisted() << endl;
    }
    
    cout << endl;
}


//
// searchClassRegArray
//
// int function to search for a perticular class in a ClassReg array
// if not found returns -1
//
int searchClassRegArray(ClassReg* classes, string courseName)
{
   string x;
    
   for (int i = 0; i < 5; ++i)
   {
      x = classes[i].getName();
       
      if (x == courseName)
      {
          return i;
      }
         
   }

   return -1;
}

//
// sortAlphabetically
//
// sorts given string array alphabetically 
//
void sortAlphabetically(string myArray[], int N)
{
   for (int i = 0; i < N-1; i++)
   {
      int minIndex = i;
      
      for (int j = i+1; j < N; j++)
      {
         if (myArray[j] < myArray[minIndex])
         {
            minIndex = j;
         }
      }
      
      string temp = myArray[i];
      myArray[i] = myArray[minIndex];
      myArray[minIndex] = temp;
   }
}


//
// list
//
// The “list” command is triggered by “l” or “list”. In response, the program outputs information about the
// specific class; if the class is not one of cs111, cs141, cs151, cs211 or cs251 then an error message is output. 
// The waitlisted students are listed in the
// same order as they are stored in the priority queue.
//
void list (ClassReg* classes, string courseName)
{
    int x = searchClassRegArray(classes, courseName);
    
    if (x == -1)
    {
        cout << "**Invalid class name, try again..." << endl << endl;
    }
    else
    {
        string CourseName, netID;
        int capacity, enrolledNum;

        CourseName = classes[x].getName();
        capacity = classes[x].getCapacity();
        enrolledNum = classes[x].numEnrolled();

        cout << CourseName << endl;
        cout << "Capacity: " << capacity << endl;
        cout << "Enrolled (" << enrolledNum << "): ";

        string* netIDArray = new string[enrolledNum];

        for (int i = 0; i < enrolledNum; ++i)
        {
            string a = classes[x].retrieveEnrolledStudent(i);
            netIDArray[i] = a;
        }

        sortAlphabetically(netIDArray, enrolledNum);

        for (int i = 0; i < enrolledNum; ++i)
        {
            cout << netIDArray[i] << " ";
        }

        cout << endl;

        int waitListedNum = classes[x].numWaitlisted();

        cout << "Waitlisted (" << waitListedNum << "): ";

        for (int i = 0; i < waitListedNum; ++i)
        {
            string netid;
            int priority;

            classes[x].retrieveWaitlistedStudent(i, netid, priority);

            cout << netid << " (" << priority << ") ";
        }
        cout << endl;
        cout << endl;
        
        delete[] netIDArray;
    }
    
}

//
// increaseClassCapacity
//
// The “increase” command is triggered by “i” or “increase”. In response, the program changes the
// enrollment capacity of the specified class; if the class is not one of cs111, cs141, cs151, cs211 or cs251 then an
// error message is output. 
//
void increaseClassCapacity (ClassReg* classes, string courseName, int newCapacity)
{
    int x = searchClassRegArray(classes, courseName);
    
    if (x == -1)
    {
        cout << "**Invalid class name, try again..." << endl << endl;
    }
    else
    {
        int oldCapacity = classes[x].getCapacity();
        
        if (newCapacity < oldCapacity)
        {
            cout << "**Error, cannot decrease class capacity, ignored..." << endl << endl;
        }
        else
        {
            classes[x].setCapacity(newCapacity);

            string CourseName;
            int capacity;

            CourseName = classes[x].getName();
            capacity = classes[x].getCapacity();

            cout << CourseName << endl;
            cout << "Capacity: " << capacity << endl << endl;
        }
          
    }  
}

//
// enrollClassNetid
//
// The “enroll” command is triggered by “e” or “enroll”. In response, the program attempts to enroll the
// student in the specified class; if the class is not one of cs111, cs141, cs151, cs211 or cs251 then an error
// message is output. If the student is already enrolled, the command has no impact (nothing changes). If the
// student is on the waitlist, then a check is made to see if there’s room in the class; if there’s room the student is
// removed from the waitlist and enrolled in the class, otherwise the command has no impact (the student
// remains on the waitlist with their original priority and position).
// 
// If the student is not enrolled and not on the waitlist, then an attempt is made to enroll them in the class. If
// the class is not full, then the student is enrolled. If the class is full, then the student is placed on the waitlist;
// what priority should be used? If the priority queue is empty, use a priority of 1, otherwise use the same
// priority as the last student in the priority queue (thereby forcing the new student to the end).
// 
void enrollClassNetid (ClassReg* classes, string courseName, string netid)
{
    int classIndex = searchClassRegArray(classes, courseName);
    
    int priority;
    
    if (classIndex == -1)
    {
        cout << "**Invalid class name, try again..." << endl;
    }
    else
    {
        int waitListSearch = classes[classIndex].searchWaitlisted(netid);
        
        if (waitListSearch != -1) 
        {
            if (classes[classIndex].numEnrolled() == classes[classIndex].getCapacity())
            {
                cout << "Class full, '" << netid <<"' waitlisted for " << classes[classIndex].getName() << endl;
            }
            else
            {
                classes[classIndex].enrollStudent(netid);
                classes[classIndex].removeWaitlistedStudent(waitListSearch);
                cout << "Student '" << netid << "' enrolled in " << classes[classIndex].getName() << endl;
            }
        }
        else if(classes[classIndex].searchEnrolled(netid) != -1)
        { //Enrolled
            cout << "Student '" << netid << "' enrolled in " << classes[classIndex].getName() << endl;
        }
        else
        {
            string netID;
            
            if (classes[classIndex].numEnrolled() == classes[classIndex].getCapacity())
            {
                if (classes[classIndex].numWaitlisted() == 0)
                {
                    classes[classIndex].waitlistStudent(netid, 1);
                    cout << "Class full, '" << netid << "' waitlisted for " << classes[classIndex].getName() << endl;
                }
                else
                {
                    int amount = classes[classIndex].numWaitlisted();
                    classes[classIndex].retrieveWaitlistedStudent(amount - 1, netID, priority);
                    classes[classIndex].waitlistStudent(netid, priority);
                    cout << "Class full, '" << netid << "' waitlisted for " << classes[classIndex].getName() << endl;
                }
            }
            else
            {
                classes[classIndex].enrollStudent(netid);
                cout << "Student '" << netid << "' enrolled in " << classes[classIndex].getName() << endl;
            }
                    
         }
       }
        
       cout << endl;
    }
    
//
//  waitlistClassNetidPriority
//
// The “waitlist” command is triggered by “w” or “waitlist”. In response, the program attempts to waitlist the
// student in the specified class; if the class is not one of cs111, cs141, cs151, cs211 or cs251 then an error
// message is output. If the student is already enrolled, the command has no impact (nothing changes). If the
// student is already waitlisted, then the student’s position in the waitlist is readjusted based on the priority. If
// the student is neither enrolled or waitlisted, they are inserted into the waitlist based on the priority.
//
void waitlistClassNetidPriority(ClassReg* classes, string courseName, string netid, int studentPriority)
{
    int classValue = searchClassRegArray(classes, courseName);
    
    if (classValue == -1)
    {
        cout << "**Invalid class name, try again..." << endl;
        return;
    }
    
    if (classes[classValue].searchEnrolled(netid) >= 0)
    {
        cout << "Student '" << netid << "' enrolled in "<< courseName << endl << endl; 
    }
    else
    {
        classes[classValue].waitlistStudent(netid, studentPriority);
        cout << "Student '" << netid << "' waitlisted for "<< courseName << endl << endl;
    }
}

//
//  process
//
// The “process” command is triggered by “p” or “process”. In response, the program attempts to open the
// given filename and input commands from this file until “q” or “quit” is encountered; you may assume the file
// contains valid commands, and ends with either “q” or “quit”. 
//
void process(istream& input, bool User, ClassReg* classes)
{
    string command, courseName, netid; 
    int newCapacity, studentPriority;
    
    if(User == true)
    {
        cout <<"Enter a command (help for more info, quit to stop)>" << endl;
    }
    
    input >> command;
    while (command != "quit" && command != "q")
    {
        if ((command == "help") || (command == "h"))
        {
            cout << "stats" << endl << "list class" << endl << "increase class capacity" << endl << "enroll class netid" << endl << "waitlist class netid priority" << endl << "process filename" << endl << "output filename" << endl << "quit" << endl << endl;
            input >> command;
        }
        
        if ((command == "stats") || (command == "s"))
        {
            stats(classes);
            input >> command;
        }
        if ((command == "list") || (command == "l"))
        {
            string courseName;
            input >> courseName;
            list(classes, courseName);
            input >> command;
        }
        if ((command == "increase") || (command == "i"))
        {
            input >> courseName;
            input >> newCapacity;
            increaseClassCapacity(classes, courseName, newCapacity);
            input >> command;
        }
        if ((command == "enroll") || (command == "e"))
        {
            input >> courseName;
            input >> netid;   
            enrollClassNetid(classes, courseName, netid);
            input >> command;
        }
        if ((command == "waitlist") || (command == "w"))
        {
            input >> courseName;
            input >> netid; 
            input >> studentPriority;
            waitlistClassNetidPriority(classes, courseName, netid, studentPriority);
            input >> command;
        }
        if ((command == "process") || (command == "p"))
        {
            string filename; input >> filename;
            ifstream infile; infile.open(filename);
            if(!infile.good())
                // Checks if file is valid
            {
                cout << "**Unable to open command file '" << filename << "'" << endl;
                input >> command;
            }
            else
            {
                cout << "**Processing commands from '" << filename << "'" << endl;
                process(infile, false, classes);
                cout << "**Done processing '" << filename << "'" << endl;
                input >> command;
            }
        }
        else if (command == "quit" && command == "q")
        {
            break;
        }
    }
        
}

//
// output
//
// The “output” command is triggered by “o” or “ouput”. In response, the program opens the given file for
// output and writes the current enrollment data to this file. The data is output in the same format as the
// enrollments input file. You may assume the file will open successfully.
//
void output(ClassReg* classes, string filename)
{
    ofstream outfile;
    outfile.open(filename);
    
    if(!outfile.good())
    {
        cout << "Error: unable to open enrollment file '" << filename << "'" << endl;
    }
    for(int i = 0; i < 5; i++)
    {
        int j;
        outfile << classes[i].getName() << endl << classes[i].getCapacity() << endl;

        for(j = 0; j < classes[i].numEnrolled(); j++)
        {
            outfile << classes[i].retrieveEnrolledStudent(j) << " ";
        }
        
        outfile << "#" << endl;

        
        for(j = 0; j < classes[i].numWaitlisted(); j++)
        {
            string studentName;
            int priority;
            classes[i].retrieveWaitlistedStudent(j, studentName, priority);
            outfile << studentName << " " << priority << " ";
        }
        outfile << "#";
        outfile << endl;
    }
    outfile.close();
    cout << "Enrollment data output to '" << filename << "'" << endl << endl;
}
    

int main()
{
    ClassReg classes[5];
    
    cout << "**CS Enrollment System**" << endl << "Enter enrollments filename> " << endl;
    
    int fileCheck = 0;
    
    inputClassInfo(classes, fileCheck);
    
    if (fileCheck == -1) // checks if file is valid
    {
        return 0;
    }
    
    
    string command, courseName, netid, filename; 
    int newCapacity, studentPriority;
    
    while (command != "quit")
    {
        cout << "Enter a command (help for more info, quit to stop)>" << endl;
        cin >> command;
        
        
        if ((command == "help") || (command == "h"))
        {
            cout << "stats" << endl << "list class" << endl << "increase class capacity" << endl << "enroll class netid" << endl << "waitlist class netid priority" << endl << "process filename" << endl << "output filename" << endl << "quit" << endl << endl;
        }
        else if (command == "quit" || command == "q")
        {
            
            break;
        }
        else if ((command == "stats") || (command == "s"))
        {
            stats(classes);
        }
        else if ((command == "list") || (command == "l"))
        {
            string courseName;
            cin >> courseName;
            list(classes, courseName);
        }
        else if ((command == "increase") || (command == "i"))
        {
            cin >> courseName;
            cin >> newCapacity;
            increaseClassCapacity(classes, courseName, newCapacity);
        }
        else if ((command == "enroll") || (command == "e"))
        {
            cin >> courseName;
            cin >> netid;   
            enrollClassNetid (classes, courseName, netid);
        }
        else if ((command == "waitlist") || (command == "w"))
        {
            cin >> courseName;
            cin >> netid; 
            cin >> studentPriority;
            waitlistClassNetidPriority(classes, courseName, netid, studentPriority);
        }
        else if ((command == "process") || (command == "p"))
        {
            string filename; cin >> filename;
            ifstream infile; infile.open(filename);
            if(!infile.good())
                // Checks if file valid
            {
                cout << "**Unable to open command file '" << filename << "'" << endl;
            }
            else
            {
                cout << "**Processing commands from '" << filename << "'" << endl;
                process(infile, false, classes);
                cout << "**Done processing '" << filename << "'" << endl;
            }
             
        }
        else if (command == "output" || command == "o")
        {
            string filename; cin >> filename;
            output(classes, filename);
                
        }
        else
        {
            cout << "**Invalid command, please try again..." << endl << endl;
        }
        
    }
   
    
    for (int i = 0; i < 5; ++i)
    {
        classes[i].~ClassReg();
    }
    
    cout << "**Done**" << endl;
    
    
    return 0;
    
}