#include <iostream>
#include <memory>
#include <vector>
using namespace std;
template <typename NeuronType>
struct NeuronBase {
    template <typename OhterNeuronType>
    void connect_to(OhterNeuronType& other) {
        for (auto&& from : *static_cast<NeuronType*>(this)) {
            for (auto&& to : other) {
                from.to.emplace_back(to.shared_from_this());
                to.from.emplace_back(from.weak_from_this());
            }
        }
    }
};

class Neuron : public enable_shared_from_this<Neuron>, public NeuronBase<Neuron> {
    uint id;

   public:
    vector<shared_ptr<Neuron>> to;
    vector<weak_ptr<Neuron>> from;
    Neuron() {
        static uint _id = 0;
        id = ++_id;
    }
    Neuron* begin() { return this; }
    Neuron* end() { return this + 1; }

    // void connect_to(Neuron& other) {
    //     to.emplace_back(other.shared_from_this());
    //     other.from.emplace_back(weak_from_this());
    // }

    friend ostream& operator<<(ostream& os, const Neuron& obj) {
        for (auto&& n : obj.from) {
            if (auto sp = n.lock(); sp != nullptr)
                os << sp->id << "\t-->\t[" << obj.id << "]" << endl;
        }

        for (auto&& n : obj.to) {
            os << "[" << obj.id << "]\t-->\t" << n->id << endl;
        }
        return os;
    }
    void print_use_count() { cout << "[" << id << "]: " << weak_from_this().use_count() << endl; }
};

class NeuronLayer : public NeuronBase<NeuronLayer> {
    using Neurons = vector<shared_ptr<Neuron>>;
    Neurons neurons_;

   public:
    struct NeuronLayerIterator {
        Neurons::iterator cur;
        explicit NeuronLayerIterator(Neurons::iterator it) : cur(it) {}
        bool operator!=(const NeuronLayerIterator& other) { return cur != other.cur; }
        NeuronLayerIterator& operator++() {
            ++cur;
            return *this;
        }
        Neuron& operator*() const { return *(cur->get()); }
        // Neuron* operator->() const { return &this->operator*(); }
    };
    NeuronLayerIterator begin() { return NeuronLayerIterator(neurons_.begin()); }
    NeuronLayerIterator end() { return NeuronLayerIterator(neurons_.end()); }

    NeuronLayer(int n) {
        while (n-- > 0)
            neurons_.emplace_back(make_shared<Neuron>());
    }
    friend ostream& operator<<(ostream& os, NeuronLayer& obj) {
        for (auto&& n : obj)
            os << n;
        return os;
    }
    void print_use_count() {
        for (auto&& p : neurons_)
            p->print_use_count();
    }
};

void test() {
    auto n1{make_shared<Neuron>()}, n2{make_shared<Neuron>()};
    n1->connect_to(*n2);
    cout << "n1:\n" << *n1 << "n2:\n" << *n2;

    auto layer1{make_shared<NeuronLayer>(5)}, layer2{make_shared<NeuronLayer>(4)};
    n1->connect_to(*layer1);
    layer1->connect_to(*layer2);
    cout << "layer1:\n" << *layer1 << "layer2:\n" << *layer2;
    n1->print_use_count();
    n2->print_use_count();
    layer1->print_use_count();
    layer2->print_use_count();
}

int main() {
    test();
    return 0;
}