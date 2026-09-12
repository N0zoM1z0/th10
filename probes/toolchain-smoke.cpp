struct FactoryToolchainPair
{
    int left;
    int right;
};

extern "C" int factory_toolchain_cpp_probe(FactoryToolchainPair *value)
{
    return value->left + value->right;
}
