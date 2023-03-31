#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <iostream>


template <typename Type>
class SingleLinkedList {
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

public:

    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;
        
        
        // Конвертирующий конструктор итератора из указателя на узел списка
        explicit BasicIterator(Node* node) {
            this->node_ = node;
        }
        
        public:
        
        using iterator_category = std::forward_iterator_tag;
        // Тип элементов, по которым перемещается итератор
        using value_type = Type;
        // Тип, используемый для хранения смещения между итераторами
        using difference_type = std::ptrdiff_t;
        // Тип указателя на итерируемое значение
        using pointer = ValueType*;
        // Тип ссылки на итерируемое значение
        using reference = ValueType&;
        
        
        BasicIterator() = default;
        
        BasicIterator(const BasicIterator<Type>& other) noexcept {
           node_ = other.node_;
        }
        
        BasicIterator& operator=(const BasicIterator& rhs) = default;
        
        // Оператор сравнения итераторов (в роли второго аргумента выступает константный итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
           return node_ == rhs.node_;
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(node_ == rhs.node_);
        }

        // Оператор сравнения итераторов (в роли второго аргумента итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
           return node_ == rhs.node_;
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(node_ == rhs.node_);
        }

        // Оператор прединкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает ссылку на самого себя
        // Инкремент итератора, не указывающего на существующий элемент списка, приводит к неопределённому поведению
        BasicIterator& operator++() noexcept {
              if(node_ != nullptr){
                  node_ = node_->next_node;
              }
              
              return *this;
        }

        // Оператор постинкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает прежнее значение итератора
        // Инкремент итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        BasicIterator operator++(int) noexcept {
           auto past = *this;
           
           if(node_ != nullptr){
               node_ = node_->next_node;
            }
              
            return past;
        }

        // Операция разыменования. Возвращает ссылку на текущий элемент
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] reference operator*() const noexcept {
            return node_->value;
        }

        // Операция доступа к члену класса. Возвращает указатель на текущий элемент списка
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] pointer operator->() const noexcept {
           return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };
        
        

    SingleLinkedList(){
        size_ = 0;
    }

    // Возвращает количество элементов в списке за время O(1)
    [[nodiscard]] size_t GetSize() const noexcept {
       return size_;
    }

    // Сообщает, пустой ли список за время O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
       return size_ == 0;
    }
    
    void PushFront(const Type& value) {
       head_.next_node = new Node(value, head_.next_node);
       size_++;
    }


    template<typename T>
    SingleLinkedList(T begin, T end){
        size_ = 0;
        Node* temp_pnt(&head_);
        try{
            for(auto it = begin; it != end; it++){
                temp_pnt->next_node = new Node(*it, nullptr);
                temp_pnt = temp_pnt->next_node;
                size_++;
            }
        }
        catch(std::bad_alloc error){
            Node* node = temp_pnt;
            while(node){
                Node* temp_node = node;
                node = node->next_node;
                delete temp_node;
            }
        }
    }

    SingleLinkedList(std::initializer_list<Type> values) {
        SingleLinkedList temp(values.begin(), values.end());
        swap(temp);
    }

    SingleLinkedList(const SingleLinkedList& other) {
        if(this != &other){
            SingleLinkedList temp(other.begin(), other.end());
            swap(temp);
        }
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if(this != &rhs){
            auto temp(rhs);

            swap(temp);
        }
        return *this;
    }

    // Обменивает содержимое списков за время O(1)
    void swap(SingleLinkedList& other) noexcept {
        // auto* temp = other.head_.next_node;
        // other.head_.next_node = head_.next_node;
        // head_.next_node = temp;
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }
    
    void Clear() noexcept {
    
        Node* next = head_.next_node;
        while(next != nullptr){
            head_.next_node = next->next_node;
            
            delete next;
            next = head_.next_node;
        }
        
        size_ = 0;
    }
    
    ~SingleLinkedList(){
        Clear();
    }
    
    
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    // Итератор, допускающий изменение элементов списка
    using Iterator = BasicIterator<Type>;
    // Константный итератор, предоставляющий доступ для чтения к элементам списка
    using ConstIterator = BasicIterator<const Type>;

    // Возвращает итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    [[nodiscard]] Iterator begin() noexcept {
        Iterator begin(head_.next_node);
        return begin;
    }

    // Возвращает итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator end() noexcept {
        Iterator end(nullptr);
        
        return end;
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    // Результат вызова эквивалентен вызову метода cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept {
        ConstIterator  begin(head_.next_node);
        return begin;
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    // Результат вызова эквивалентен вызову метода cend()
    [[nodiscard]] ConstIterator end() const noexcept {
        ConstIterator  end(nullptr);
        
        return end;
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept {
       ConstIterator  begin(head_.next_node);
        return begin;
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cend() const noexcept {
        ConstIterator  end;
        
        return end;
    }
    
    [[nodiscard]] Iterator before_begin() noexcept {
        //Node* temp = &head
        return Iterator(&head_);
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        //Node* const node = &head_; 
        return ConstIterator(temp_head);
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator(&head_);
    }

    /*
     * Вставляет элемент value после элемента, на который указывает pos.
     * Возвращает итератор на вставленный элемент
     * Если при создании элемента будет выброшено исключение, список останется в прежнем состоянии
     */
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        Node* node = new Node(value, pos.node_->next_node);
        pos.node_->next_node = node;
        size_++;
        return Iterator(node);
    }

    

    /*
     * Удаляет элемент, следующий за pos.
     * Возвращает итератор на элемент, следующий за удалённым
     */
    Iterator EraseAfter(ConstIterator pos) noexcept {
        Node* temp = pos.node_->next_node;
        temp = temp->next_node;

        delete pos.node_->next_node;

        pos.node_->next_node = temp;

        size_--;

        return Iterator(temp);
    }

    void PopFront() noexcept {
        // Node* node = head_.next_node;
        // node = node->next_node;
        // delete head_.next_node;
        // head_.next_node = node; 
        EraseAfter(before_begin());
    }

private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    Node* const temp_head = &head_;
    size_t size_;
};


template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    
    return (lhs.GetSize() == rhs.GetSize()) && std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {

    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (lhs < rhs) || (lhs == rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (rhs < lhs) || (lhs == rhs);
} 

