#ifndef __PATH_H__
#define __PATH_H__

#include <list>
#include <cassert>

#include <glm/vec3.hpp>

class Path
{
private:

    std::list<glm::vec3>            m_WayPoints;

    //points to the current waypoint
    std::list<glm::vec3>::iterator  curWaypoint;

    //flag to indicate if the path should be looped
    //(The last waypoint connected to the first)
    bool                           m_bLooped;

public:

    //constructor for creating a path with initial random waypoints. MinX/Y
    //& MaxX/Y define the bounding box of the path.
    Path() :m_bLooped(true)
    {
        m_WayPoints.clear();

        m_WayPoints.push_back(glm::vec3(-100.0f, 2.5f, 25.0f));
        m_WayPoints.push_back(glm::vec3(-100.0f, 2.5f, 40.0f));
        // m_WayPoints.push_back(glm::vec3(0.0f, 1.5f, 10.0f));
        // m_WayPoints.push_back(glm::vec3(0.0f, 1.5f, 0.0f));

        curWaypoint = m_WayPoints.begin();
    }


    //returns the current waypoint
    glm::vec3    CurrentWaypoint()const { return *curWaypoint; }

    //returns true if the end of the list has been reached
    bool        Finished() { return !(curWaypoint != m_WayPoints.end()); }

    //moves the iterator on to the next waypoint in the list
    inline void SetNextWaypoint();

    void LoopOn() { m_bLooped = true; }
    void LoopOff() { m_bLooped = false; }

    //adds a waypoint to the end of the path
    void AddWayPoint(glm::vec3 new_point)
    {
        m_WayPoints.push_back(new_point);
    }

    //methods for setting the path with either another Path or a list of vectors
    void Set(std::list<glm::vec3> new_path) { m_WayPoints = new_path; curWaypoint = m_WayPoints.begin(); }
    void Set(const Path& path) { m_WayPoints = path.GetPath(); curWaypoint = m_WayPoints.begin(); }


    void Clear() { m_WayPoints.clear(); }

    std::list<glm::vec3> GetPath()const { return m_WayPoints; }

};

inline void Path::SetNextWaypoint()
{
    assert(m_WayPoints.size() > 0);

    std::cout << "Next target" << std::endl;

    if (++curWaypoint == m_WayPoints.end())
    {
        if (m_bLooped)
        {
            curWaypoint = m_WayPoints.begin();
        }
    }
}


#endif