#pragma once
#include <memory>
#include <algorithm>
#include "InputAction.hpp"



class LeftistHeap {
public:
    LeftistHeap() = default;
    LeftistHeap(const LeftistHeap& other);
    LeftistHeap& operator=(const LeftistHeap& other);

    bool empty() const;
    void insert(std::unique_ptr<InputAction> action);
    std::unique_ptr<InputAction> extractMin();
    const InputAction* peekMin() const;

    // Шаблонный метод traverse должен быть определён здесь, чтобы компилятор мог его инстанцировать.
    template<typename Func>
    void traverse(Func&& f) const {
        traverseNode(root_.get(), std::forward<Func>(f));
    }

private:
    struct Node {
        int rank_;
        std::unique_ptr<InputAction> action_;
        std::unique_ptr<Node> left_;
        std::unique_ptr<Node> right_;

        Node(std::unique_ptr<InputAction> action, std::unique_ptr<Node> left, std::unique_ptr<Node> right)
            : action_(std::move(action)), left_(std::move(left)), right_(std::move(right)) {
            rank_ = 1 + std::min(LeftistHeap::getRank(left_.get()), LeftistHeap::getRank(right_.get()));
        }
    };

    std::unique_ptr<Node> root_;
    static int getRank(const Node* node);
    static std::unique_ptr<Node> mergeNodes(std::unique_ptr<Node> a, std::unique_ptr<Node> b);
    static std::unique_ptr<Node> cloneNode(const std::unique_ptr<Node>& n);

    // Вспомогательный рекурсивный обход
    template<typename Func>
    static void traverseNode(const Node* n, Func&& f) {
        if (!n) return;
        f(n->action_.get());
        traverseNode(n->left_.get(),  std::forward<Func>(f));
        traverseNode(n->right_.get(), std::forward<Func>(f));
    }

};
