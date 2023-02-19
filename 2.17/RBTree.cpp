#include<iostream>
#include "RBTree.h"

int RBTreeNode::getNodeVal() {
    return Val;
}

//获取祖父节点
RBTreeNode* RBTreeNode::getGrandparent() {
    return this->patent->parent;
}

//获取叔父节点
RBTreeNode* RBTreeNode::getUncle() {
    //如果父亲是爷爷节点的左孩子
    if(this->parent == this->getGrandparent()->left) {
        return this->getGrandparent()->right;
    }else if(this-parent == this->getGrandparent()->right) {
        return this->getGrandparent()->left;
    }else{
        return NULL;
    }
}

//创建红黑树，返回红黑树的根
RBTreeNode* RBTreeNode::createRBTree() {
    
}

//销毁红黑树
void RBTreeNode::destoryRBTree() {

}

//插入节点到红黑树中，成功返回0，失败返回-1
int RBTreeNode::insertRBNode(RBTreeNode* root, Type key) {

}


//删除节点（key为节点的值）
void RBTreeNode::deleteRBNode(RBTreeNode* root, Type key){

}



int main() {

    return 0;
}