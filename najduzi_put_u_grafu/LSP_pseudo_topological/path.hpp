#pragma once
#include <deque>
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <queue>
#include <algorithm>
#include <cassert>

using node_t = int;

const node_t INVALID_NODE = -1;

using weight_t = int;

// something larger than weight_t for when you have that weight_t doesn't properly hold a sum of weight_t
using sumweight_t = int;
const sumweight_t INF = 1000000000;

struct NeighborNode
{
	explicit NeighborNode() : node(INVALID_NODE), weight(0) {} 
	
	explicit NeighborNode(node_t v, weight_t w) : node(v), weight(w) {}
	
	inline operator node_t() const
	{
		return node;
	}
	
	weight_t Weight() const
	{
		return weight;
// 		return 1;
	}
	
	node_t node;
	weight_t weight{1}; //comment 
};

class Path
{
public:
    Path() : m_path(), m_explored(), m_value(0) {}
	
	Path(size_t n) : m_path(), m_explored(n), m_value(0) {}
    Path(size_t n, node_t initialnode) : m_path(), m_explored(n), m_value(0) 
    {
        emplace_back(initialnode,0);
    }
    
    
    const std::deque<NeighborNode>& get_path() const { return m_path; }
    inline operator const std::deque<NeighborNode>&() const { return m_path; }
//     inline operator vector<NeighborNode>&()  { return m_path; }
    sumweight_t value() const { return m_value; }
    sumweight_t cost() const { return m_value; }
    sumweight_t weight() const { return m_value; }
    
    bool is_node_in_path(node_t v) const
	{
		return m_explored[v] > 0;
	}
    
    void push_back(const NeighborNode& v)
    {
        assert(v.weight == 0 || !m_path.empty());
        assert(v < m_explored.size());

        ++m_explored[v];
        m_value += v.weight;
        m_path.push_back(v);
    }
    
    void emplace_back(node_t node, weight_t weight = 1)
    {
        assert(weight == 0 || !m_path.empty());
        assert(node < m_explored.size());
        ++m_explored[node];
        m_value += weight;
        m_path.emplace_back(node,weight);
    }
    
    void push_front(NeighborNode v)
	{
//         swap(m_path.front().weight,v.weight);
        assert(!m_path.empty() && "Use push_front when it's empty");
        assert(v < m_explored.size());
        m_path.front().weight = v.weight;
        m_path.emplace_front(v,0);
		m_value += v.weight;
        ++m_explored[v];
	}
	
    void emplace_front(node_t node, weight_t weight = 1)
	{
//         auto w = m_path.front().weight;
        assert(node < m_explored.size());
		if (!m_path.empty())
			m_path.front().weight = weight;
		m_path.emplace_front(node,0);
		m_value += weight;
        ++m_explored[node];
	}
    
    void pop_back()
    {
        assert(!m_path.empty() && "Can't pop when it's already empty!");
        auto v = m_path.back();
        --m_explored[v];
        m_value -= v.weight;
        m_path.pop_back();
    }
    
    void pop_front()
    {
        assert(!m_path.empty() && "Can't pop when it's already empty!");
        auto v = m_path.front();
        --m_explored[v];
        m_path.pop_front();
        m_value -= m_path.front().weight;
        m_path.front().weight = 0;
    }
    
    void clear()
    {
        m_value = 0;
//         m_explored = vector<char>(m_explored.size(),0);
        auto n = m_explored.size();
        m_explored.clear();
        m_explored.resize(n,0);
        m_path.clear();
    }
    
    NeighborNode operator[](size_t i) const { return m_path[i]; }
    NeighborNode& operator[](size_t i) { return m_path[i]; }
    
    bool empty() const
    {
        return m_path.empty();
    }
    
    size_t size() const
    {
        return m_path.size();
    }
    
    template <class Compare>
    NeighborNode first_not_explored_binary(const std::vector<NeighborNode>& Nodes, node_t start, Compare comp) const
    {
        auto it = std::upper_bound(Nodes.begin(), Nodes.end(), start, comp);
    // 	++it;
        while (it != Nodes.end() && m_explored[*it])
            ++it;
        if (it == Nodes.end())
            return NeighborNode(INVALID_NODE,0);
        return *it;
    }
    
    NeighborNode first_not_explored_binary(const std::vector<NeighborNode>& Nodes, node_t start) const
    {
        return first_not_explored_binary(Nodes,start,std::less<node_t>());
    }
    
    NeighborNode first_not_explored(const std::vector<NeighborNode>& Nodes, node_t start) const
    {
        bool seenstart = false;
        for (auto x : Nodes)
        {
            if (x == start)
            {
                seenstart = true;
                continue;
            }	
            
            if (seenstart && !m_explored[x])
                return x;
        }
        return NeighborNode(INVALID_NODE,0);
    }
    
    NeighborNode first_not_explored(const std::vector<NeighborNode>& Nodes) const
    {
        for (auto x : Nodes)
        {
            if (!m_explored[x])
                return x;
        }
        return NeighborNode(INVALID_NODE,0);
    }
    
    NeighborNode back() const
    {
        return m_path.back();
    }
    
    NeighborNode front() const
    {
        return m_path.front();
    }
    
public:
    std::deque<NeighborNode> m_path;
private:
    std::vector<char> m_explored;
//     std::vector<bool> m_explored;
    sumweight_t m_value;
public:
    const decltype(m_path)& data() const { return m_path; }

};

// inline std::ostream& operator<<(std::ostream& os, const Path& P)
// {
//     auto B = P.get_path();
//     for (size_t i = 0; i < B.size()-1; ++i)
// 	{
// 		os << P.get_digraph()->get_vertex_name(B[i]) << u8" ??? ";
// 	}
// 	os << P.get_digraph()->get_vertex_name(B.back()) << endl;
// 	return os;
// }

struct Edge
{
	Edge() : from(INVALID_NODE), to(INVALID_NODE), weight(0) {}
	Edge(node_t f, node_t t, weight_t w = 1) : from(f), to(t), weight(w) {}
	node_t operator[](bool i)
	{
		if (i)
			return to;
		return from;
	}
	node_t from;
	node_t to;
	weight_t weight;
};

/*
class Path
{
public:
    Path(const DiGraph* parent, node_t node) :m_parent(parent),
													m_totalvalue(0),
													m_path(1,node),
													m_explored(parent->get_size(),0)
	{
		m_explored[node] = 1;
	}
	
	Path(const DiGraph*const parent, const std::deque<node_t>& path);
	Path(const DiGraph*const parent, const std::deque<node_t>& path, sumweight_t value);
	
    const DiGraph* const get_digraph() const { return m_parent; }
    
	bool DFSBackNext(int maxbacktrack = 99999);
	bool DFSFrontNext(int maxbacktrack = 99999);
	
	void AddNodeBack(node_t t);
	void AddNodeFront(node_t t);
	
	void PopBack();
	void PopFront();
	void printMovies();
	void Reset();
	
	sumweight_t Value() const { return m_totalvalue; }
	
	void ExpandGreedyBack();
	void ExpandGreedyFront();
	
	node_t FirstNotExplored(const std::vector<node_t>& Nodes) const
	{
		for (auto x : Nodes)
		{
			if (m_explored[x] == false)
				return x;
		}
		return INVALID_NODE;
	}
	
	bool SanityCheck() const;
	
	node_t FirstNotExplored(const std::vector< node_t >& Nodes, node_t start) const;
	int FirstNotExploredEX(const std::vector< node_t >& Nodes, node_t start) const;
	int FirstNotExploredIN(const std::vector< node_t >& Nodes, node_t start) const;
// 	int LengthOfFirstRunNI(const Path& P) const;
	
	// destroys path
	Path BestCompletion();
	
	bool operator==(const Path& P) const;
	bool operator!=(const Path& P) const;
	const std::deque<node_t>& get_path() const { return m_path; }
	bool IsNodeInPath(node_t node) const { return m_explored[node]; }
    void transform_nodes(const std::vector< node_t >& m_removalfunctioninverse);
	
private:
	const DiGraph* m_parent;
    sumweight_t m_totalvalue;
	std::deque<node_t> m_path;
	std::vector<char> m_explored;
};*/

// std::ostream& operator<<(std::ostream& os, const Path& P);

