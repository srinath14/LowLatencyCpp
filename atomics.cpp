#include <iostream>
#include <atomic>

struct Node{
    int val;
    Node* next{nullptr};
}
std::atomic<Node*>pHead(nullptr);
void push(int& value){
    Node* newNode = new Node();//thread1, thread2
    newNode->val = value;//thread1, thread21,2

    //Node* expected= ;//nullptr , nullptr

    do{
        newNode->next=pHead.load();
    }while(!pHead.compare_exchange_strong(newNode->next,newNode));


}
/// 1--->2---->3----->4----->5
void pop(){

    Node* temp ;
    do{

        temp = pHead.load();
        if(!temp)return;
    }while(!pHead.compare_exchange_strong(temp, temp->next))
    //pHead = temp->next;

    //delete temp;



}


int main(){


    return 0;
}