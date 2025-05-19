#include "LeftistHeap.hpp"

LeftistHeap::LeftistHeap(const LeftistHeap& other) : root_(cloneNode(other.root_)){}

LeftistHeap& LeftistHeap::operator=(const LeftistHeap& other) {
    if (this != &other) {
        root_.reset();
        root_ = cloneNode(other.root_);
    }
    return *this;
}


int LeftistHeap::getRank(const Node* node) {
    return node ? node->rank_ : 0;
}

std::unique_ptr<LeftistHeap::Node> LeftistHeap::mergeNodes(std::unique_ptr<Node> a, std::unique_ptr<Node> b) {
    if (!a) return b;
    if (!b) return a;

    if (*a->action_ < *b->action_) {
        a->right_ = mergeNodes(std::move(a->right_), std::move(b));
        if (getRank(a->left_.get()) < getRank(a->right_.get())) {
            std::swap(a->left_, a->right_);
        }
        a->rank_ = 1 + getRank(a->right_.get());
        return a;
    } else {
        b->right_ = mergeNodes(std::move(a), std::move(b->right_));
        if (getRank(b->left_.get()) < getRank(b->right_.get())) {
            std::swap(b->left_, b->right_);
        }
        b->rank_ = 1 + getRank(b->right_.get());
        return b;
    }
}

std::unique_ptr<LeftistHeap::Node> LeftistHeap::cloneNode(const std::unique_ptr<Node>& n) {
    if (!n) return nullptr;
    auto action_clone = std::unique_ptr<InputAction>( n->action_->clone() );
    auto left_clone  = cloneNode(n->left_);
    auto right_clone = cloneNode(n->right_);
    return std::make_unique<Node>(
        std::move(action_clone),
        std::move(left_clone),
        std::move(right_clone)
    );
}


bool LeftistHeap::empty() const {
    return !root_;
}

void LeftistHeap::insert(std::unique_ptr<InputAction> action) {
    auto new_node = std::make_unique<Node>(std::move(action), nullptr, nullptr);
    root_ = mergeNodes(std::move(root_), std::move(new_node));
}

std::unique_ptr<InputAction> LeftistHeap::extractMin() {
    if (!root_) return nullptr;
    auto min_action = std::move(root_->action_);
    root_ = mergeNodes(std::move(root_->left_), std::move(root_->right_));
    return min_action;
}

const InputAction* LeftistHeap::peekMin() const {
    return root_ ? root_->action_.get() : nullptr;
}

