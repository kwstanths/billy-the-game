#ifndef __List_hpp__
#define __List_hpp__

#include <iostream>


namespace game_engine {

namespace utility {

namespace list {

	template <typename T> class list;

	template <typename T> class list_iterator;

	template <typename T> class list_node{
		friend class list<T>;
		friend class list_iterator<T>;
		private:
			list_node(const T& _value, list_node<T> * _next, list_node<T> * _previous): value(_value), next(_next), previous(_previous){}
			T value;
			list_node<T> * next;
			list_node<T> * previous;
		public:
			T getValue(){
				return value;
			}
	};

	template <class T> class list{
	private:
		list_node<T> * top, * bot;

	public:
		/*
			list's iterator
		*/
		typedef list_iterator<T> iterator;


		/*
			Default constructor. Initializes an empty list
		*/
		list(): top(NULL), bot(NULL){ }


		/*
			Initializes a list with a single element
			@param _value The initial value
		*/
		list(T _value){
			list_node<T> * new_node = new list_node<T>(_value, NULL, NULL);
			top = new_node;
			bot = new_node;
		}


		/*
			Destroys the list
		*/
		~list(){
			Clear();
		}


		/*
			Returns if the list is empty
			@return true if empty, false if not
		*/
		bool IsEmpty(){
			return ((top == NULL) || (bot == NULL));
		}


		/*
			Returns the size of the list
			@return the size of the list
		*/
		int Size(){
			list_node<T> * temp_node = top;
			int sum = 0;
			while(temp_node != NULL){
				sum++;
				temp_node = temp_node->next;
			}
			return sum;
		}


		/*
			Removes every element in the list
		*/
		void Clear(){
			list_node<T> * temp_node;
			while(top != NULL){
				temp_node = top;
				top = top->next;
				delete temp_node;
			}
			top = bot = NULL;
		}


		/*
			Searches the list forwards to find _value
			@param _value the element to find
			@return index of the element if found, otherwise -1
		*/
		int FindForward(T _value){
			if (IsEmpty()) return -1;
			int index = 0;
			list_node<T> * temp_node = top;
			do{
				if (temp_node->value == _value) return index;
				temp_node = temp_node->next;
				index++;
			}while(temp_node != NULL);

			return -1;
		}


		/*
			Searches the list backwards to find _value
			@param _value the element to find
			@return index of the element if found, otherwide -1
		*/
		int FindBackward(T _value){
			if (IsEmpty()) return -1;
			int index = 0;
			list_node<T> * temp_node = bot;
			do{
				if (temp_node->value == _value) return index;
				temp_node = temp_node->previous;
				index++;
			}while(temp_node!=NULL);

			return -1;
		}


		/*
			Inserts at the top of the list
			@param _value the element to insert
		*/
		void PushTop(T _value){

			list_node<T> * new_node = new list_node<T>(_value, top, NULL);
			top = new_node;
			if (top->next != NULL) (top->next)->previous = new_node;
			if (bot == NULL) bot = new_node;
		}


		/*
			Inserts at the bottom of the list
			@param _value the element to insert
		*/
		void PushBottom(T _value){
			list_node<T> * new_node = new list_node<T>(_value,NULL, bot);
			if (top == NULL) {
				top = new_node;
				bot = new_node;
				return;
			}
			bot->next = new_node;
			bot = new_node;
			return;
		}


		/*
			Removes an element from the top of the list. If none exists, the default T() element 
			will be returned.
			@return The value removed
		*/
		T PopTop(){
			if (IsEmpty()) return T();

			list_node<T> * temp_node = top;
			T temp_value = temp_node->value;
			top = top->next;
			if (top != NULL) top->previous = NULL;
			if (top == NULL) bot = NULL;

			delete temp_node;
			return temp_value;

		}


		/*
			Removes an element from the bottom of the list. If none exists, the default T() element
			will be returned
			@return The value removed
		*/
		T PopBottom(){
			if (IsEmpty()) return T();

			list_node<T> * temp_node = bot;
			T temp_value = temp_node->value;

			bot = bot->previous;
			if (bot!=NULL) bot->next = NULL;
			if (bot == NULL) top = NULL;

			return temp_value;
		}


		/*
			Inserts an element in the list at a certain index
			@param position the index to insert the element
			@param _value the element to insert
			@return true if inserted, false if not
		*/
		bool Insert(int _position, T _value){
			if (IsEmpty() && _position != 0) return false;

			if (_position == 0) {
				PushTop(_value);
				return true;
			}
			int index = 1;
			list_node<T> * current;
			current = top->next;
			while((index < _position) && (current != NULL)){
				index++;
				current = current->next;
			}

			if((current == NULL) && (index < _position)) return false;
			if((current == NULL) && (index == _position)) {
				PushBottom(_value);
				return true;
			}
			if((current != NULL) && (index == _position)){
				list_node<T> * temp_node = new list_node<T>(_value,current,current->previous);
				(current->previous)->next = temp_node;
				current->previous = temp_node;
				return true;
			}
		}


		/*
			Inserts an element in the list after a specific value
			@param _previous_value the value after of which the elemen will be inserted
			@param _value the element to insert
			@return true if inserted, false if not
		*/
		bool InsertAfter(T _previous_value, T _value){
			if (IsEmpty()) return false;

			if (bot->value  == _previous_value) {
				PushBottom(_value);
				return true;
			}
			list_node<T> * previous, * temp_node;

			previous = top;
			while(previous != NULL){
				if (previous->value == _previous_value) {
					temp_node = new list_node<T>(_value, previous->next, previous);
					previous->next = temp_node;
					(previous->next)->previous = temp_node;
					return true;
				};
				previous = previous->next;
			}

			return false;
		}


		/*
			Insert an element in the list before a specific value
			@param _next_value the value before of which the elemened will be inserted
			@param _value the element to insert
			@return true if inserted, false if not
		*/
		bool InsertBefore(T _next_value, T _value){
			if (IsEmpty()) return false;

			if (top->value == _next_value) {
				PushTop(_value);
				return true;
			}
			list_node<T> * previous, * current, * temp_node;
			previous = top;
			current = top->next;
			while(current != NULL){
				if (current->value == _next_value){
					temp_node = new list_node<T>(_value,current, previous);
					current->previous = temp_node;
					previous->next = temp_node;
					return true;
				}
				previous = current;
				current = current->next;
			}

			return false;
		}


		/*
			Remove an element from the list_node
			@param _value the element to remove
			@return true if removed, false if not
		*/
		bool Remove(T _value){
			if (IsEmpty()) return false;

			if (top->value == _value) {
				PopTop();
				return true;
			}
			if (bot->value == _value){
				PopBottom();
				return true;
			}

			list_node<T> * previous, * current, * temp_node;
			previous = top;
			current = top->next;
			while(current != NULL){
				if (current->value == _value){
					temp_node = current;
					previous->next = current->next;
					(current->next)->previous = previous;
					delete temp_node;
					return true;
				}
				previous = current;
				current = current->next;
			}

			return false;
		}


		/*
			Remove an element from the list in the position index from the front
			@param _index The position of the element to remove
			@return true = Removed, false = Not removed
		*/
		bool RemoveIndex(int _index){
			if (IsEmpty()) return false;

			if (_index == 0) {
				PopTop();
				return true;
			}

			list_node<T> * current, * temp_node;
			int i = 1;
			current = top;
			while(current != NULL && i <= _index){
				current = current->next;
				i++;
			}

			if (current == NULL) return false;
			temp_node = current;
			(current->previous)->next = current->next;
			if (current->next == NULL) {
				bot = current->previous;
			}else (current->next)->previous = current->previous;

			delete temp_node;
			return true;
		}


		/*
			Get an iterator to the start of the list
		*/
		iterator begin() {
			return list_iterator<T>(top);
		}


		/*
			Get an iterator to the end of the list
		*/
		iterator end(){
			return list_iterator<T>(NULL);
		}


		/*
			Returns a pointer the top node
		*/
		auto PeekTop() -> list_node<T> * {
			return top;
		}


		/*
			Returns a pointer to the last node
		*/
		auto PeekBottom() ->list_node<T> * {
			return bot;
		}


		/*
			Returns a pointer to the index list node or NULL
		*/
		auto PeekIndex(int _index) -> list_node<T> * {

			if (IsEmpty()) return NULL;

			if (_index == 0) return top;

			list_node<T> * current;
			int i = 1;
			current = top->next;
			while(current != NULL && i < _index){
				current = current->next;
				i++;
			}
			return current;
		}


		/*
			Forward print of the elements in the list
		*/
		void PrintForward(){
			list_node<T> * temp = top;
			std::cout << "Forwards - List: ";
			while(temp != NULL){
				std::cout << temp->value << " " ;
				temp = temp->next;
			}
			std::cout << std::endl;
		}


		/*
			Backward print of the elements in the list
		*/
		void PrintBackward(){
			list_node<T> * temp = bot;
			std::cout << "Backwards - List: ";
			while(temp != NULL){
				std::cout << temp->value << " " ;
				temp = temp->previous;
			}
			std::cout << std::endl;
		}

	};

	template <typename T> class list_iterator : public std::iterator<std::forward_iterator_tag, T>{
		friend class list<T>;
	private:
		list_node<T> *pointed;
		list_iterator(list_node<T> * _pointed): pointed(_pointed){ }

	public:
		T& operator*(){
			return pointed->value;
		}

		const list_iterator<T>& operator++(){
			pointed = pointed->next;
			return *this;
		}

		const list_iterator<T>& operator+=(int amount){
			int i = 0;
			while(i++ < amount) {
				pointed = pointed->next;
			}
		}

		bool operator!=(const list_iterator<T>& other) const {
			return this->pointed != other.pointed;
		}

	};

} /* list namespace */

} /* utility namespace */

} /* game engine namespace */

#endif
