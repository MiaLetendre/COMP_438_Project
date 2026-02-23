#pragma once

#include "geometry.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

namespace vine {

// Forward declarations
struct HalfEdge;
struct Vertex;
struct Face;

// ---- Vertex ---------------------------------------------------------------
struct Vertex {
    Vec3      position;
    HalfEdge* halfEdge{nullptr};  // one outgoing half-edge from this vertex
};

// ---- Half-Edge ------------------------------------------------------------
struct HalfEdge {
    Vertex*   origin{nullptr};  // start vertex
    HalfEdge* twin{nullptr};    // opposite half-edge
    HalfEdge* next{nullptr};    // next half-edge around the face (CCW)
    HalfEdge* prev{nullptr};    // previous half-edge around the face
    Face*     face{nullptr};    // incident face (nullptr = boundary)
};

// ---- Face -----------------------------------------------------------------
struct Face {
    HalfEdge* halfEdge{nullptr};  // one bounding half-edge

    // Collect all half-edges around this face
    std::vector<HalfEdge*> edges() const;

    // Compute the face normal (Newell's method)
    Vec3 normal() const;
};

// ---- HalfEdgeMesh ---------------------------------------------------------
class HalfEdgeMesh {
public:
    HalfEdgeMesh() = default;

    // Add an isolated vertex; returns a pointer to it
    Vertex* addVertex(Vec3 position);

    // Add a polygonal face from an ordered list of vertices (CCW)
    // Twins are linked automatically when both faces sharing an edge exist
    Face* addFace(const std::vector<Vertex*>& verts);

    // Accessors
    const std::vector<std::unique_ptr<Vertex>>&   vertices()  const;
    const std::vector<std::unique_ptr<HalfEdge>>& halfEdges() const;
    const std::vector<std::unique_ptr<Face>>&     faces()     const;

private:
    std::vector<std::unique_ptr<Vertex>>   m_vertices;
    std::vector<std::unique_ptr<HalfEdge>> m_halfEdges;
    std::vector<std::unique_ptr<Face>>     m_faces;

    // (from, to) vertex index -> half-edge; used to pair twins
    std::unordered_map<std::size_t, HalfEdge*> m_edgeMap;
};

} // namespace vine
