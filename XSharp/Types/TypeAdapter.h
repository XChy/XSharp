namespace XSharp {
class TypeNode;

class TypeAdapter
{
   public:
    static bool canConvert(TypeNode* from, TypeNode* to);
};

}  // namespace XSharp
