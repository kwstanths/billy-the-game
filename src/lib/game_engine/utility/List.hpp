#ifndef __List_hpp__
#define __List_hpp__

#include <iostream>


namespace game_engine {

namespace utility {

	/**
		list container
		TODO FindAndRemove(...)
		TODO Backwards iteration
	*/
	template <typename T> class List {
	private:

		class ListNode {
			friend class List;
		private:
			ListNode(T _value, ListNode * _next, ListNode * _previous): value(_value), next(_next), previous(_previous){}
			T value;
			ListNode * next;
			ListNode * previous;
		public:
			T& getValue(){
				return value;
			}
		};

		class ListIterator : public std::iterator<std::forward_iterator_tag, T> {
			friend class List;
			friend class ListNode;
		private:
			ListNode * pointed_;
			ListIterator(ListNode * pointed): pointed_(pointed){ }

		public:
			T& operator*(){
				return pointed_->value;
			}

			const ListIterator& operator++(){
				pointed_ = pointed_->next;
				return *this;
			}

			const ListIterator& operator+=(int amount){
				int i = 0;
				while(i++ < amount) {
					pointed_ = pointed_->next;
				}
			}

			bool operator!=(const ListIterator& other) const {
				return this->pointed_ != other.pointed_;
			}

			bool operator==(const ListIterator& other) const {
				return this->pointed_ == other.pointed_;
			}
		};

		ListNode * top, * bot;

	public:
		/*
			list's iterator
		*/
		typedef ListIterator iterator;


		/*
			Default constructor. Initializes an empty list
		*/
		List(): top(NULL), bot(NULL){ }


		/*
			Initializes a list with a single element
			@param _value The initial value
		*/
		List(T _value){
			ListNode * new_node = new ListNode(_value, NULL, NULL);
			top = new_node;
			bot = new_node;
		}


		/*
			Destroys the list
		*/
		~List(){
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
			ListNode * temp_node = top;
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
			ListNode * temp_node;
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
		ListIterator FindForward(T _value){
			if (IsEmpty()) return end();
			
			ListNode * temp_node = top;
			do {
				if (temp_node->value == _value) return ListIterator(temp_node);
				temp_node = temp_node->next;
			} while (temp_node != NULL);

			return end();
		}


		/*
			Searches the list backwards to find _value
			@param _value the element to find
			@return index of the element if found, otherwide -1
		*/
		ListIterator FindBackward(T _value){
			if (IsEmpty()) return end();
			
			ListNode * temp_node = bot;
			do {
				if (temp_node->value == _value) return ListIterator(temp_node);
				temp_node = temp_node->previous;
				
			} while(temp_node!=NULL);

			return end();
		}


		/*
			Inserts at the top of the list
			@param _value the element to insert
		*/
		void PushTop(T _value){

			ListNode * new_node = new ListNode(_value, top, NULL);
			top = new_node;
			if (top->next != NULL) (top->next)->previous = new_node;
			if (bot == NULL) bot = new_node;
		}


		/*
			Inserts at the bottom of the list
			@param _value the element to insert
		*/
		void PushBottom(T _value){
			ListNode * new_node = new ListNode(_value,NULL, bot);
			if (top == NULL) {
				top = new_node;
				bot = new_node;
				return;
			}
			bot->next = new_node;
			bot = new_node;
			return;
		}

		/**
		
		*/

		/*
			Removes the element on the top of the list
		*/
		void PopTop() {
			if (IsEmpty()) return;

			ListNode * temp_node = top;
			T temp_value = temp_node->value;
			top = top->next;
			if (top != NULL) top->previous = NULL;
			if (top == NULL) bot = NULL;

			delete temp_node;
		}


		/*
			Removes the element from the bottom of the list
		*/
		void PopBottom() {
			if (IsEmpty()) return;

			ListNode * temp_node = bot;
			T temp_value = temp_node->value;

			bot = bot->previous;
			if (bot!=NULL) bot->next = NULL;
			if (bot == NULL) top = NULL;

			delete temp_node;
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
			ListNode * current;
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
				ListNode * temp_node = new ListNode(_value,current,current->previous);
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
			ListNode * previous, * temp_node;

			previous = top;
			while(previous != NULL){
				if (previous->value == _previous_value) {
					temp_node = new ListNode(_value, previous->next, previous);
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
			ListNode * previous, * current, * temp_node;
			previous = top;
			current = top->next;
			while(current != NULL){
				if (current->value == _next_value){
					temp_node = new ListNode(_value,current, previous);
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
			Remove an element from the ListNode
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

			ListNode * previous, * current, * temp_node;
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

			ListNode * current, * temp_node;
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
		ListIterator begin() {
			return ListIterator(top);
		}


		/*
			Get an iterator to the end of the list
		*/
		ListIterator end(){
			return ListIterator(NULL);
		}


		/*
			Returns a raw pointer the top element. If list is empty you get nullptr
			@return A pointer to the first data in the list
		*/
		T * PeekTop() {
			if (IsEmpty()) return nullptr;
			return &top->value;
		}


		/*
			Returns a raw pointer to the last element. If list is empty you get nullptr
			@return A pointer to the last data in the list
		*/
		T * PeekBottom() {
			if (IsEmpty()) return nullptr;
			return &bot->value;
		}

		/*
			Returns a raw pointer to the element in the list index or NULL if such does not exist
		*/
		T * PeekIndex(int _index) {

			if (IsEmpty()) return NULL;

			if (_index == 0) return &top->getValue();

			ListNode * current;
			int i = 1;
			current = top->next;
			while(current != NULL && i < _index){
				current = current->next;
				i++;
			}
			return &current->getValue();
		}


		/*
			Forward print of the elements in the list
		*/
		void PrintForward(){
			ListNode * temp = top;
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
			ListNode * temp = bot;
			std::cout << "Backwards - List: ";
			while(temp != NULL){
				std::cout << temp->value << " " ;
				temp = temp->previous;
			}
			std::cout << std::endl;
		}

	};

} /* utility namespace */

} /* game engine namespace */

#endif
