#include <tuple>
#include <vector>
#include <algorithm>
#include <cassert>
#include <stdexcept>
//#include <iostream>
/**
 * An abstract template base of the KDTree class
 */
template<typename...>
class KDTree;

/**
 * A partial template specialization of the KDTree class
 * The time complexity of functions are based on n and k
 * n is the size of the KDTree
 * k is the number of dimensions
 * @typedef Key         key type
 * @typedef Value       value type
 * @typedef Data        key-value pair
 * @static  KeySize     k (number of dimensions)
 */
template<typename ValueType, typename... KeyTypes>
class KDTree<std::tuple<KeyTypes...>, ValueType> {
public:
    typedef std::tuple<KeyTypes...> Key;
    typedef ValueType Value;
    typedef std::pair<const Key, Value> Data;
    static inline constexpr size_t KeySize = std::tuple_size<Key>::value;
    static_assert(KeySize > 0, "Can not construct KDTree with zero dimension");
protected:
    struct Node {
        Data data;
        Node *parent;
        Node *left = nullptr;
        Node *right = nullptr;

        Node(const Key &key, const Value &value, Node *parent) : data(key, value), parent(parent) {}

        const Key &key() { return data.first; }

        Value &value() { return data.second; }
    };

public:
    /**
     * A bi-directional iterator for the KDTree
     * ! ONLY NEED TO MODIFY increment and decrement !
     */
    class Iterator {
    private:
        KDTree *tree;
        Node *node;

        Iterator(KDTree *tree, Node *node) : tree(tree), node(node) {}

        /**
         * Increment the iterator
         * Time complexity: O(log n)
         */
        void increment() {
            if (node->right) {
                node = node->right;
                while (node->left) node = node->left;
            } else {
                while (node->parent && node->parent->right == node) node = node->parent;
                node = node->parent;
            }
            // TODO: implement this function
        }

        /**
         * Decrement the iterator
         * Time complexity: O(log n)
         */
        void decrement() {
            if (node->left) {
                node = node->left;
                while (node->right) node = node->right;
            } else {
                while (node->parent &&node->parent->left == node) node = node->parent;
                node = node->parent;
            }
            // TODO: implement this function
        }

    public:
        friend class KDTree;

        Iterator() = delete;

        Iterator(const Iterator &) = default;

        Iterator &operator=(const Iterator &) = default;

        Iterator &operator++() {
            increment();
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            increment();
            return temp;
        }

        Iterator &operator--() {
            decrement();
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            decrement();
            return temp;
        }

        bool operator==(const Iterator &that) const {
            return node == that.node;
        }

        bool operator!=(const Iterator &that) const {
            return node != that.node;
        }

        Data *operator->() {
            return &(node->data);
        }

        Data &operator*() {
            return node->data;
        }
    };

protected:                      // DO NOT USE private HERE!
    Node *root = nullptr;       // root of the tree
    size_t treeSize = 0;        // size of the tree

    /**
     * Find the node with key
     * Time Complexity: O(k log n)
     * @tparam DIM current dimension of node
     * @param key
     * @param node
     * @return the node with key, or nullptr if not found
     */
    template<size_t DIM>
    Node *find(const Key &key, Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (node == nullptr) {
            return nullptr;
        }
        if (node->key() == key) {
            return node;
        }
        if (compareKey<DIM, std::less<>>(key, node->key())) {
            return find<DIM_NEXT>(key, node->left);
        } else {
            return find<DIM_NEXT>(key, node->right);
        }

        // TODO: implement this function
    }

    /**
     * Insert the key-value pair, if the key already exists, replace the value only
     * Time Complexity: O(k log n)
     * @tparam DIM current dimension of node
     * @param key
     * @param value
     * @param node
     * @param parent
     * @return whether insertion took place (return false if the key already exists)
     */
    template<size_t DIM>
    bool insert(const Key &key, const Value &value, Node *&node, Node *parent) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (node == nullptr) {
            node = new Node(key, value, parent);
            treeSize++;
            return true;
        }
        if (key == node->key()) {
            node->value() = value;
            return false;
        }
        if (compareKey<DIM, std::less<>>(key, node->key())) {
            return insert<DIM_NEXT>(key, value, node->left, node);
        } else {
            return insert<DIM_NEXT>(key, value, node->right, node);

        }
        // TODO: implement this function
    }

    /**
     * Compare two keys on a dimension
     * Time Complexity: O(1)
     * @tparam DIM comparison dimension
     * @tparam Compare
     * @param a
     * @param b
     * @param compare
     * @return relationship of two keys on a dimension with the compare function
     */
    template<size_t DIM, typename Compare>
    static bool compareKey(const Key &a, const Key &b, Compare compare = Compare()) {
        if (std::get<DIM>(a) != std::get<DIM>(b)) {
            return compare(std::get<DIM>(a), std::get<DIM>(b));
        }
        return compare(a, b);
    }

    /**
     * Compare two nodes on a dimension
     * Time Complexity: O(1)
     * @tparam DIM comparison dimension
     * @tparam Compare
     * @param a
     * @param b
     * @param compare
     * @return the minimum / maximum of two nodes
     */
    template<size_t DIM, typename Compare>
    static Node *compareNode(Node *a, Node *b, Compare compare = Compare()) {
        if (!a) return b;
        if (!b) return a;
        return compareKey<DIM, Compare>(a->key(), b->key(), compare) ? a : b;
    }

    /**
     * Find the minimum node on a dimension
     * Time Complexity: ?
     * @tparam DIM_CMP comparison dimension
     * @tparam DIM current dimension of node
     * @param node
     * @return the minimum node on a dimension
     */
    template<size_t DIM_CMP, size_t DIM>
    Node *findMin(Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (node == nullptr) {
            return nullptr;
        }
        Node *left = findMin<DIM_CMP, DIM_NEXT>(node->left);
        if (DIM_CMP != DIM) {
            Node *right = findMin<DIM_CMP, DIM_NEXT>(node->right);
            left = compareNode<DIM_CMP, std::less<>>(left, right);
        }
        return compareNode<DIM_CMP, std::less<>>(left, node);
        // TODO: implement this function
    }

    /**
     * Find the maximum node on a dimension
     * Time Complexity: ?
     * @tparam DIM_CMP comparison dimension
     * @tparam DIM current dimension of node
     * @param node
     * @return the maximum node on a dimension
     */
    template<size_t DIM_CMP, size_t DIM>
    Node *findMax(Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (node == nullptr) {
            return nullptr;
        }
        Node *right = findMax<DIM_CMP, DIM_NEXT>(node->right);
        if (DIM_CMP != DIM) {
            Node *left = findMax<DIM_CMP, DIM_NEXT>(node->left);
            right = compareNode<DIM_CMP, std::greater<>>(left, right);
        }
        return compareNode<DIM_CMP, std::greater<>>(right, node);
        // TODO: implement this function
    }

    template<size_t DIM>
    Node *findMinDynamic(size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return findMin<DIM, 0>(root);
        return findMinDynamic<DIM_NEXT>(dim);
    }

    template<size_t DIM>
    Node *findMaxDynamic(size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return findMax<DIM, 0>(root);
        return findMaxDynamic<DIM_NEXT>(dim);
    }

    /**
     * Erase a node with key (check the pseudocode in project description)
     * Time Complexity: max{O(k log n), O(findMin)}
     * @tparam DIM current dimension of node
     * @param node
     * @param key
     * @return nullptr if node is erased, else the (probably) replaced node
     */
    template<size_t DIM>
    Node *erase(Node *node, const Key &key) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (node == nullptr) {
            return nullptr;
        }
        if (key == node->key()) {
            if ((node->left == nullptr) && (node->right == nullptr)) {
                delete node;
                treeSize--;
                return nullptr;
            } else {
                if (node->right != nullptr) {
                    Node *minNode = findMin<DIM, DIM_NEXT>(node->right);
                    const Key *const_pointer = &node->data.first;
                    Key *modifier = const_cast<Key *>(const_pointer);
                    *modifier = minNode->key();
                    node->value()= minNode->value();
                    node->right= erase<DIM_NEXT>(node->right,minNode->key());
                } else {
                    Node *maxNode = findMax<DIM, DIM_NEXT>(node->left);
                    const Key *const_pointer = &node->data.first;
                    Key *modifier = const_cast<Key*>(const_pointer);
                    *modifier = maxNode->key();
                    node->value() = maxNode->value();
                    node->left= erase<DIM_NEXT>(node->left, maxNode->key());
                }
            }
        } else {
            if (compareKey<DIM, std::less<>>(key, node->key())) {
                node->left = erase<DIM_NEXT>(node->left, key);
            } else {
                node->right = erase<DIM_NEXT>(node->right, key);
            }
        }
        return node;
        // TODO: implement this function
    }

    template<size_t DIM>
    Node *eraseDynamic(Node *node, size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return erase<DIM>(node, node->key());
        return eraseDynamic<DIM_NEXT>(node, dim);
    }

    void copy_helper(Node *node, const Node *that) {
        if (that->left != nullptr) {
            insert(that->left->key(), that->left->value());
            copy_helper(node->left, that->left);
        }
        if (that->right != nullptr) {
            insert(that->right->key(), that->right->value());
            copy_helper(node->right, that->right);
        }
    }

    template<size_t DIM>
    Node *vector_helper(std::vector<std::pair<Key, Value>> v, Node *parent) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (v.empty()) {
            return nullptr;
        }
//        std::stable_sort(v.begin(),v.end(),[](std::pair<Key, Value> a,std::pair<Key, Value> b){return compareKey<DIM,std::less<>>((Key)a.first,(Key)b.first);});
//        std::unique(v.rbegin(),v.rend(),[](std::pair<Key, Value> a,std::pair<Key, Value> b){return a.first==b.first;});
        std::nth_element(v.begin(), v.begin() + (size_t) ((v.size() - 1) / 2), v.end(),
                         std::less<>());
        Node *node=new Node((v.begin() + (v.size() - 1) / 2)->first,((v.begin() + (v.size() - 1) / 2))->second,parent);
        std::vector<std::pair<Key, Value>> left;
        left.assign(v.begin(), v.begin() + (v.size() - 1) / 2);
        std::vector<std::pair<Key, Value>> right;
        right.assign(v.begin() + (v.size() + 1) / 2, v.end());
        node->left = vector_helper<DIM_NEXT>(left, node);
        node->right = vector_helper<DIM_NEXT>(right, node);

        return node;
    }

    void destruct_helper(Node *node) {
        if (node == nullptr) {
            return;
        }
        destruct_helper(node->left);
        destruct_helper(node->right);
        delete node;
    }

    // TODO: define your helper functions here if necessary

public:
    KDTree() = default;

    /**
     * Time complexity: O(kn log n)
     * @param v we pass by value here because v need to be modified
     */
    explicit KDTree(std::vector<std::pair<Key, Value>> v) {
       // using namespace  std;
        std::stable_sort(v.begin(),v.end(),[](const std::pair<Key, Value>& a,const std::pair<Key, Value>& b){return compareKey<0,std::less<>>((Key)a.first,(Key)b.first);});
//        cout<<"sort"<<std::endl;
//        for (auto [p,va]: v) {
//            auto [x,y]=p;
//            cout<<x<<","<<y<<","<<va<<endl;
//        }
        typename std::vector<std::pair<Key, Value>> ::reverse_iterator p=std::unique(v.rbegin(),v.rend(),[](const std::pair<Key, Value>& a,const std::pair<Key, Value>& b){return a.first==b.first;});
//        cout<<"unique"<<std::endl;
//        for (auto [p,va]: v) {
//            auto [x,y]=p;
//            cout<<x<<","<<y<<","<<va<<endl;
//        }
        typename std::vector<std::pair<Key, Value>> ::iterator i(p.base());
        v.erase(v.begin(),i);
//        cout<<"after"<<endl;
//        for (auto [p,va]: v) {
//            auto [x,y]=p;
//            std::cout<<x<<","<<y<<","<<va<<std::endl;
//        }
        treeSize = v.size();
//        root= nullptr;
//        for (auto i:v) {
//            insert(i.first,i.second);
//        }
        //std::stable_sort(v.begin(),v.end(),compareKey<>())
        root = vector_helper<0>(v, nullptr);
        //root= nullptr;

        // TODO: implement this function
    }

    /**
     * Time complexity: O(n)
     */
    KDTree(const KDTree &that) :
            treeSize(that.treeSize) {
        root = new Node(that.root->key(), that.root->value(), nullptr);
        copy_helper(root, that.root);
        // TODO: implement this function
    }

    /**
     * Time complexity: O(n)
     */
    KDTree &operator=(const KDTree &that) {
        if (&that==this) return *this;
        this->treeSize = that.treeSize;
        Node *temp = new Node(that.root->key(), that.root->value(), nullptr);
        Node *root_backup=root;
        root=temp;
        copy_helper(temp, that.root);
        destruct_helper(root_backup);
        return *this;
        // TODO: implement this function
    }

    /**
     * Time complexity: O(n)
     */
    ~KDTree() {
        destruct_helper(root);
        treeSize=0;
        // TODO: implement this function
    }


    Iterator begin() {
        if (!root) return end();
        auto node = root;
        while (node->left) node = node->left;
        return Iterator(this, node);
    }

    Iterator end() {
        return Iterator(this, nullptr);
    }

    Iterator find(const Key &key) {
        return Iterator(this, find<0>(key, root));
    }

    void insert(const Key &key, const Value &value) {
        insert<0>(key, value, root, nullptr);
    }

    template<size_t DIM>
    Iterator findMin() {
        return Iterator(this, findMin<DIM, 0>(root));
    }

    Iterator findMin(size_t dim) {
        return Iterator(this, findMinDynamic<0>(dim));
    }

    template<size_t DIM>
    Iterator findMax() {
        return Iterator(this, findMax<DIM, 0>(root));
    }

    Iterator findMax(size_t dim) {
        return Iterator(this, findMaxDynamic<0>(dim));
    }

    bool erase(const Key &key) {
        auto prevSize = treeSize;
        erase<0>(root, key);
        return prevSize > treeSize;
    }

    Iterator erase(Iterator it) {
        if (it == end()) return it;
        auto node = it.node;
        if (!it.node->left && !it.node->right) {
            it.node = it.node->parent;
        }
        size_t depth = 0;
        auto temp = node->parent;
        while (temp) {
            temp = temp->parent;
            ++depth;
        }
        eraseDynamic<0>(node, depth % KeySize);
        return it;
    }

    size_t size() const { return treeSize; }
};
