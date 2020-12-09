#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
struct Range{
    int min, max;
    Range(int min, int max): min(min), max(max){}
    static Range * commonPart(const Range * r1, const Range * r2){
        int min = r1->min > r2->min ? r1->min : r2->min;
        int max = r1->max < r2->max ? r1->max : r2->max;
        return new Range(min, max);
    }
};
struct SegmentTree{
    struct BinaryNode{
        const int id;
        Range * range;
        bool colours[3] = {false};//yellow, blue, red
        BinaryNode(int id) : id(id){};
        ~BinaryNode(){
            delete range;
        }
    };
    vector<BinaryNode *> nodes;
    int height;
    int firstFloorSize;
    SegmentTree(int dataLength){
        float logVal = log2(dataLength);
        this->height = (int)logVal + (((int)logVal) != logVal) + 1;
        this->firstFloorSize = pow(2, height - 1);
        int totalNodes = pow(2, height) - 1;
        for(int i = 0; i < totalNodes; i ++){
            nodes.push_back(new BinaryNode(i));
        }
        for(int i = 0; i < firstFloorSize; i ++){
            int nodeID = nodes.size() - 1 - i;
            nodes[nodeID]->range = new Range(firstFloorSize - 1 - i, firstFloorSize - 1 - i);
        }
        for(int i = 0; i < nodes.size() - firstFloorSize; i ++){
            int nodeID = nodes.size() - firstFloorSize - 1 - i;
            BinaryNode * leftChild = nodes[getChildID(nodeID, true)];
            BinaryNode * rightChild = nodes[getChildID(nodeID, false)];
            nodes[nodeID]->range = new Range(leftChild->range->min, rightChild->range->max);
        }
        cout<<"";
    }
    static int getChildID(int parentID, bool left){
        return parentID * 2 + 1 + !left;
    }
    static int getParentID(int childID){
        return (childID - 1) / 2;
    }
    void propagate(Range * range, int colour, int nodeID = 0){
        BinaryNode * node = nodes[nodeID];
        if(node->range->min == range->min && node->range->max == range->max){
            node->colours[colour] = true;
        }
        else if(getChildID(nodeID, false) < nodes.size() && range->min <= range->max){
            int leftID = getChildID(nodeID, true);
            int rightID = getChildID(nodeID, false);
            Range * commonLeft = Range::commonPart(range, nodes[leftID]->range);
            Range * commonRight = Range::commonPart(range, nodes[rightID]->range);
            propagate(commonLeft, colour, leftID);
            propagate(commonRight, colour, rightID);
            delete  commonLeft;
            delete commonRight;
        }
    }
    bool isGreen(int leafID){
        int nodeID = nodes.size() - firstFloorSize + leafID;
        BinaryNode * node = nodes[nodeID];
        bool colours[3] = {false};
        while(true){
            for(int i = 0; i < 3; i ++){
                colours[i] += node->colours[i];
            }
            if(getParentID(node->id) == node->id)
                break;
            node = nodes[getParentID(node->id)];
        }
        return colours[0] && colours[1] && !colours[2];
    }
    ~SegmentTree(){
        for(auto n : nodes){
            delete n;
        }
    }
};
int main() {
    std::ios_base::sync_with_stdio(false);
    int length;
    int queries;
    cin >> length;
    cin >> queries;
    SegmentTree sg = SegmentTree(length);
    for(int i = 0; i < queries; i ++){
        int min, max, colour;
        cin >> min;
        cin >> max;
        cin >> colour;
        Range * r = new Range(--min, --max);
        sg.propagate(r, --colour);
        delete r;
    }

    int sum = 0;
    for(int i = 0; i < length; i ++){
        sum += sg.isGreen(i);//
    }
    cout<<sum;
    return 0;
}
