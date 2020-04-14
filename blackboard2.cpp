#include <iostream>
#include <vector>
#include <chrono>

#define MAX_DEQUE_SIZE 100

inline void error(const char* str){
    std::cerr << str << "\n"; 
    exit(1);
};


template <typename Type>
class node{  
    public:
        Type data;
        node* next;
        node* prev;
        node(){}
        node(Type data){
            this->data = data;
            next = nullptr;
            prev = nullptr;
        }
        ~node(){}
};

template <typename Type>
class d_linked_list{
    protected:
        node<Type>* head;
        node<Type>* tail;  // head: first node, tail: last node
        int length;

    public:
        d_linked_list(){ // 생성자
            head = nullptr;
            tail = nullptr;
            this &head->prev = nullptr;
            this &tail->next = nullptr;
            length = 0;
        }   
        ~d_linked_list(){ // 소멸자
            delete_list();
        }   

        // 삽입연산
        void push_front(Type data){  
            // Creating new node
            node<Type> *tmp = this->node(data);   
            tmp->prev = nullptr;   
            tmp->next = head;
            length++;

            // if list is empty
            if(head == nullptr){ tail = tmp; }
            else{ head->prev = tmp; }
            head = tmp;
        }
        
        void push_back(Type data){   
            // creating new node
            node<Type> *tmp = this->node(data);   
            length++;
            tmp->data = data;
            tmp->prev = tail;
            tmp->next = nullptr;
            
            // if list is empty
            if(tail == nullptr){ head = tmp; }
            else{ tail->next = tmp;}
            tail = tmp;
        } 
		
        void insert_node_at(int idx, Type data){ 
            if(length < idx || 0 > idx){
                std::cout << "해당 인덱스는 존재하지 않습니다\n";
            }
            else if(idx == 0){  
                push_front(data);
            }
            else if(idx == length){  
                push_back(data);
            }
            else{
                node<Type> *ptr = head;
                node<Type> *tmp = ptr;
                node<Type> *new_node = this->node(data);
                new_node->data = data;
                new_node->next = nullptr; // 새 node를 일단 초기화해줍니다.
                for(int i=0; i < idx; i++){
                    tmp = ptr; // 들어갈 노드의 직전 노드 위치
                    ptr = ptr->next; // 들어갈 노드의 위치
                }
                tmp->next = new_node;
                new_node->next = ptr;
                length++; // 노드를 삽입합니다.
            }
        }

        // 삭제연산
        Type pop_front(){ 
            if(head == nullptr){
                std::cout << "linked list가 비어있습니다\n";
                exit(1);
            }
            node<Type> *ptr = head;
            Type value = head->data;
            head = ptr->next;
            if(ptr == tail){  
                head = nullptr;
                tail = nullptr;
            }
            else{ head->prev = nullptr; }
            delete ptr;
            length--;
            return value;
        } 
        
        Type pop_back(){ // 맨 뒤 노드를 지우고, 그 값을 반환합니다.
            if(length == 0){  
                std::cout << "linked list가 비어있습니다\n";
                exit(1);
            }
            node<Type> *ptr = tail;
            Type value = tail->data;
            tail = ptr->prev;
            if(ptr == head){  
                head = nullptr;
                tail = nullptr;
            }
            else{ tail->next = nullptr; }
            delete ptr;
            length--;
            return value;
        } 
        
        Type pop_node_at(int idx){ // idx 번째 노드를 지우고, 그 값을 반환합니다.
            Type value = -1;
            if(length <= idx || 0 > idx){  
                std::cout << "해당 인덱스는 존재하지 않습니다\n";
            }
            else if(length == 0){  // 리스트가 비었을때 pop_node_at()을 시도한 경우
                std::cout << "linked list가 비어있습니다\n";
                exit(1);
            }
            else if(idx == 0){ 
                value = pop_front();
            }
            else if(idx == length - 1){ 
                value = pop_back();
            }
            else{
                node<Type> *ptr = head;
                node<Type> *tmp = ptr;
                for(int i = 0; i < idx; i++){
                    tmp = ptr; // 현재 노드를 tmp로 두고
                    ptr = ptr->next; // 다음 노드로 타겟을 바꿉니다.
                }
                //std::cout << ptr->data << "값의 노드를 하나 지웁니다\n";
                value = ptr->data;
                tmp->next = ptr->next; // 삭제할 노드를 빼고 연결하고
                if(ptr == tail){
                    tail = tmp;
                }
                length--; // 노드를 삽입합니다.
                delete ptr;
                ptr = nullptr;
            }
            return value;
        } 
        
        void delete_list(){ // linked list 전체를 지웁니다. 
            node<Type> *ptr = head;
            while(ptr != nullptr){
                head = ptr->next;
                delete ptr;
                ptr = head;
            }
            length = 0;
            std::cout << "리스트가 삭제되었습니다 \n";
        }
        
        // 조회연산
        Type front(){ // 맨 앞 노드를 지우지 말고 반환합니다.
            if(length == 0){ // *** head == nullptr과 length == 0 간에는 어떤 의미 차이가 있습니까? 둘 다 리스트가 빈 것을 의미하지 않나요? *** 
                std::cout << "linked list가 비어있습니다\n";
                exit(1);
            }
            return head->data;
        } 

        Type back(){ // 맨 뒤 노드를 지우지 말고 반환합니다.
            if(length == 0){  // 리스트가 비었을때 pop_back()을 시도한 경우
                std::cout << "linked list가 비어있습니다\n";
                exit(1);
            }
            return tail->data;
        } 

        int size(){ // linked list 전체의 노드 수를 반환합니다.
            return length;
        } 
};

template <typename Type>
class dll_deque: public d_linked_list<Type>{
    public:
        Type data[MAX_DEQUE_SIZE];
        dll_deque(node* head, node* tail, int length, Type data[MAX_DEQUE_SIZE]):d_linked_list(), data[MAX_DEQUE_SIZE]{ }
        //dll_deque():d_linked_list(), data[MAX_DEQUE_SIZE]{ }
        ~dll_deque(){
            delete data[MAX_DEQUE_SIZE]; // *** 이렇게 쓰면 삭제가 될까요...?
        }
        void addFront(Type data){ // 데이터를 맨 앞에 한 개 추가합니다.
            d_linked_list<Type>::push_front(data); // fix
        } 
        
        void addRear(Type data){ // 데이터를 맨 뒤에 한 개 추가합니다. // fix
            d_linked_list<Type>::push_back(data); 
        } 

        Type getFront(){ // 맨 앞 데이터의 내용을 반환합니다.
            return  d_linked_list<Type>::front();
        } 

        Type getRear(){ // 맨 뒤 데이터의 내용을 반환합니다.
            return  d_linked_list<Type>::back();
        } 

        Type deleteFront(){ // 맨 앞 데이터를 지우고 내용을 반환합니다.
            return  d_linked_list<Type>::pop_front();
        } 

        Type deleteRear(){ // 맨 뒤 데이터를 지우고 내용을 반환합니다.
            return  d_linked_list<Type>::pop_back();
        }
};


template <typename Type>
int main(){
    dll_deque deq[MAX_DEQUE_SIZE]<int>;
    for(int i=1 ; i<10 ; i++) {
        if(i % 2 == 0) {deq.addFront(i);}
        else{deq.addRear(i);}
    }
    deq.deleteFront();
    deq.deleteRear();
    deq.deleteFront();
    deq.getFront();
    deq.getRear();
    printf("fix\n");
    return 0;
}