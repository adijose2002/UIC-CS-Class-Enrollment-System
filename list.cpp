/*list.cpp*/

//
// Author: Prof. Hummel, U. of I. Chicago, Spring 2021
// 
// Implements a one-way linked-list with optimized insertion at
// head and tail of list. The nodes contain 2 data values, a string
// and an integer.
//

#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>

#include "list.h"

using namespace std;

//
// private member functions:
//

//
// inits this list then makes deep copy of other
//
// Time complexity: O(N)
//
void List::initAndDeepCopy(const List& other)
{
   // Setting elements of List structs empty.
    this->Head = NULL;
    this->Count = 0;
    
    Node* otherCur = other.Head;
    
    while(otherCur != NULL)
    {
        this->push_back(otherCur->Data.Value1, otherCur->Data.Value2);
        
        otherCur = otherCur->Next;
    }
}


// 
// free the nodes in the list and resets to empty
//
// Time complexity: O(N)
//
void List::freeAndReset()
{ 
    Node* curr = this->Head;
    Node* temp = NULL;
    while(curr != NULL) // deleting each node through iteration
    {
        
        temp = curr;
        curr = curr->Next;
        delete temp;
    }
    // freeing memory
    this->Head = NULL;
    this->Tail = NULL;
    this->Count = 0;
}


//
// public member functions:
//

//
// default constructor
//
// Initializes the list to empty.
// 
// Time complexity: O(1)
//
List::List()
{
   this->Head = NULL;
   this->Tail = NULL;
   this->Count = 0;
}


//
// copy constructor
//
// Makes a deep copy of the other list into this list.
// 
// Time complexity: O(N)
//
List::List(const List& other)
{
   this->initAndDeepCopy(other);
}


//
// destructor
//
// Frees all memory related to this list, and then resets to empty.
// 
// Time complexity: O(N)
//
List::~List()
{
   this->freeAndReset();
}


//
// assignment operator (e.g. L2 = L;)
//
// Makes a deep copy of the other list into this list; since this
// list already exists, the existing elements of this list are freed
// before the copy is made.
// 
// Time complexity: O(N)
//
List& List::operator=(const List& other)
{
   if (this == &other)
   {
      return *this; 
   }

   this->freeAndReset();
   this->initAndDeepCopy(other);

   return *this;  
}


//
// size
//
// Returns the # of elements in the list.
//
// Time complexity: O(1)
//
int List::size()
{
   return this->Count;
}


//
// empty
//
// Returns true if empty, false if not.
//
// Time complexity: O(1)
//
bool List::empty()
{
    if(this->Head == NULL && this->Count == 0)
    {
        return true;
    }
    return false;
}


//
// search
//
// Search the list for the first occurence of the string value.
// If found, its position in the list is returned. Positions are 
// 0-based, meaning the first node is position 0. If the value is
// not found, -1 is returned.
//
// Time complexity: on average O(N)
//
int List::search(string value)
{
    Node* cur = this->Head;
    int i, index = -1; // returning int object to adjust later
    for(i = 0; i < this->Count; ++i)
    {
        
        if(cur->Data.Value1 == value)
        {
            index++;
            return index; // final int returned
        }
        else
        {
            
        index++;
        cur = cur->Next;
        }
    }
    
    return -1; // ID not found 
}


//
// retrieve
//
// Retrieve's the data from node at the given position; the list 
// remains unchanged. Positions are 0-based, meaning the first node
// is position 0. Throws an "invalid_argument" exception if the 
// position is invalid, i.e. 
// throw invalid_argument("List::retrieve: invalid position");
//
// Time complexity: on average O(N)
//
void List::retrieve(int pos, string& value1, int& value2)
{
    if(pos >= this->Count || pos < 0)   
    {
    throw invalid_argument("List::retrieve: invalid position");    
    }
    
    Node* cur = this->Head;
    for(int i = 0; i < pos; ++i)
    {
        cur = cur->Next;
    }
    value1 =  cur->Data.Value1;
    value2 = cur->Data.Value2;
}


//
// insert
//
// Inserts the given data in the list such that after
// the insertion, the value is now at the given
// position.
//
// Positions are 0-based, which means a position of 0 
// denotes the data will end up at the head of the list,
// and a position of N (where N = the size of the list)
// denotes the data will end up at the tail of the list.
// If the position is invalid, throws an exception, i.e.
// throw invalid_argument("List::insert: invalid position");
//
// Time complexity: on average O(N)
//
void List::insert(int pos, string value1, int value2)
{
    if(pos < 0 || pos > this->Count)
    {
        
        throw invalid_argument("List::insert: invalid position");
    }
    
    if(pos == 0)
    {
        this->push_front(value1, value2);
    }
    
    else if(pos == this->Count)
    {
        this->push_back(value1, value2);
    }
    
    else
    {
        Node* curr = this->Head;
        for(int i = 0; i < pos -1; ++i)
        {
            curr = curr->Next;
        }
        Node* newN = new Node();
        
        newN->Data.Value1 = value1;
        newN->Data.Value2 = value2;
        
        newN->Next = curr->Next;
        curr->Next = newN;
        this->Count++;
    }
}


//
// remove
//
// Removes and deletes the node at the given position; no data is
// returned. Positions are 0-based, meaning the first node
// is position 0. Throws an "invalid_argument" exception if the 
// position is invalid, i.e. 
// throw invalid_argument("List::remove: invalid position");
//
// Time complexity: on average O(N)
//
void List::remove(int pos)
{
    
    if(pos >= this->Count || pos < 0)
    {
        throw invalid_argument("List::remove: invalid position");
    }
    
    Node* cur = this->Head;
    if(pos == 0) // at beggining of list 
    {
        if(this->Count == 1)
        {
            Node* cur = this->Head;
            delete cur; this->Head = NULL; this->Tail = NULL;
            this->Count--;
        }
        else if(this->Count == 2)
        {
            Node* cur = this->Head;
            
            this->Head = cur->Next;
            
            delete cur;
            
            cur = this->Head;
            
            this->Tail = cur;
            
            this->Count--;
        }
        else
        {
            Node* cur = this->Head;
            
            this->Head = cur->Next;
            
            delete cur;
            
            this->Count--;
        }
    }
    else if (pos + 1 == this->Count)
    {
        
        Node* temp = NULL;
       
        while(cur->Next != NULL) // iterating through the list
        {
            temp = cur;
            cur = cur->Next;
        }
        
        temp->Next = NULL;
        
        this->Tail = temp;
        
        delete cur;
        
        this->Count--;
    }
    else
    {
        
        Node* temp = NULL;
        
        for(int i = 0; i < pos ; ++i)
        {
            temp = cur;
            cur = cur->Next;
        }
        
        temp->Next = cur->Next;
        
        delete cur;
        
        this->Count--; // lowering count as each node is removed
    }
}


//
// front
//
// Returns the data from the first node in the list. Throws an 
// exception if the list is empty, i.e.
// throw runtime_error("List::front: empty list");
// 
// Time complexity: O(1)
//
void List::front(string& value1, int& value2)
{
    
    if(this->Count == 0 && this->Head == NULL)
    {
    throw invalid_argument("List::front: empty list");
    }
    else
    {
        value1 = this->Head->Data.Value1;
        value2 = this->Head->Data.Value2;
    }
}


//
// back
//
// Returns the data from the last node in the list. Throws an 
// exception if the list is empty, i.e.
// throw runtime_error("List::back: empty list");
// 
// Time complexity: O(1)
//
void List::back(string& value1, int& value2)
{
    if(this->Count == 0 && this->Head == NULL)
    {
        throw invalid_argument("List::back: empty list");
    }
    else
    {
        Node* cur = this->Tail; 
        value1 = cur->Data.Value1;
        value2 = cur->Data.Value2;
    }
}


//
// push_front
//
// Inserts the given data at the front of the list.
// 
// Time complexity: O(1)
//
void List::push_front(string value1, int value2)
{
  
    Node* newN = new Node();
    
    newN->Data.Value1 = value1;
    newN->Data.Value2 = value2;
    
    newN->Next = this->Head; 
    
    this->Head = newN;
    
    if(this->Count == 0)
    {
        this->Tail = newN;
    }
    
    this->Count++;
}


//
// push_back
//
// Inserts the given data at the back of the list.
// 
// Time complexity: O(1)
//
void List::push_back(string value1, int value2)
{
    
    Node* newNode = new Node();
    newNode->Data.Value1 = value1;
    newNode->Data.Value2 = value2;
    newNode->Next = NULL;
    
    if(this->Head == NULL) // reached back of list
    {
        this->Head = newNode;
        this->Tail = newNode;
    }
    else // not back of list keep iterating
    {
        this->Tail->Next = newNode;
        this->Tail = this->Tail->Next;
    }
    this->Count++; // increment counting as each node is inserted
}