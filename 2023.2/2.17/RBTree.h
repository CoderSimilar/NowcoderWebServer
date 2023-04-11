//防卫式声明
#ifdef _RBTREE_H
#define _RBTREE_H
typedef int Type;
#include<iostream>
#define RED 0   //红色节点
#define BLACK 1 //黑色节点

//红黑树的节点
class RBTreeNode{

private:
    unsigned char Color;    //颜色(RED/BLACK)
    Type key;               //关键值
    RBTreeNode* left;       //左孩子
    RBTreeNode* right;      //右孩子
    RBTreeNode* parent;     //父节点

public:
    RBTreeNode(): Val(0), Color(BLACK), left(nullptr), right(nullptr), parent(nullptr){}
    RBTreeNode(int val): Val(val), Color(BLACK), left(nullptr), right(nullptr), parent(nullptr){}
    RBTreeNode(int val, RBTreeNode* left, RBTreeNode* right): Val(val), Color(BLACK), left(left), right(right), parent(nullptr){}
    RBTreeNode(int val, int color, RBTreeNode* left, RBTreeNode* right): Val(val), Color(BLACK), left(left), right(right), parent(nullptr){}

    //获取本节点的值
    int getNodeVal();

    //获取祖父节点
    RBTreeNode* getGrandparent();

    //获取叔父节点
    RBTreeNode* getUncle();

    //创建红黑树，返回红黑树的根
    RBTreeNode* createRBTree();

    //销毁红黑树
    void destoryRBTree();

    //插入节点到红黑树中，成功返回0，失败返回-1
    int insertRBNode(RBTreeNode* root, Type key);

    //删除节点（key为节点的值）
    void deleteRBNode(RBTreeNode* root, Type key);

    
    //前序遍历红黑树
    void preorderRBTree();

    //中序遍历红黑树
    void inorderRBTree();

    //后序遍历红黑树
    void postorderRBTree();

    //(递归实现)查找红黑树中键值为key的节点，找到返回0，失败返回-1
    int searchRBTree(RBTreeNode* root, Type key);

    //(非递归实现)查找红黑树中键值为key的节点，找到返回0，失败返回-1
    int iterativeSearchRBTree(RBTreeNode* root, Type key);

    //返回最小节点的值
    int minRBTreeNode(RBTreeNode* root, int& val);

    //返回最大节点的值
    int maxRBTreeNode(RBTreeNode* root, int& val);

    //打印红黑树
    void printRBTree(RBTreeNode* root);
};

//红黑树的根节点
class RBROOT : RBTreeNode {
public:
    RBTreeNode* node;
}

#endif