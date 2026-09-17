#pragma once

struct LzssTreeNode
{
    int parent;
    int left;
    int right;
};

typedef char LzssTreeNodeSizeIs0C[(sizeof(LzssTreeNode) == 0x0c) ? 1 : -1];

extern LzssTreeNode g_LzssTree[0x2001];

unsigned char *CompressData(unsigned char *input, int inputSize, int *outputSize);

class Lzss
{
  public:
    static void InitTree(int root);
    static void InitEncoderState();
    static int AddString(int newNode, int *matchPosition);
    static void __fastcall DeleteString(int node);
    static void ContractNode(int oldNode, int newNode);
    static void ReplaceNode(int oldNode, int newNode);
    static int FindNextNode(int node);
};
