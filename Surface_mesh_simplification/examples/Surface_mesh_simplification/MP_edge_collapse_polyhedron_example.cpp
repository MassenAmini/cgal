// Edge-length/Midpoint edge-collapse with ordinary surface and external per-edge pointer.
//
// Explicit arguments:
// 
//   The surface is an ordinary Polyhedron_3
//
//   The stop condition is to finish when the number of undirected edges 
//   drops below a certain relartive fraction of the initial count.
//
//   An external map is used to store the per-edge extra pointer unintrusively.
//
//   No vertex is fixed.
//
//   No collapsed data is cached (tha collapse_data is an empty struct)
//
//   The cost is given by the squared length of the edge and is calculated on demand.
// 
//   The placement is given as the midpoint of the collapsed edge and is calculated on demand.  
// 
//
// Implicit arguments:
//
//   No visitor is passed.
//
#include <iostream>
#include <iomanip>
#include <fstream>

#include <CGAL/Simple_cartesian.h>

// Target surface type. (this include Polyhedron_3.h itself)
#include <CGAL/Surface_mesh_simplification/Polyhedron.h>

// Policies
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Midpoint_and_length.h>  //<==== NOTICE THIS 
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_ratio_stop_pred.h>

// Simplification method
#include <CGAL/Surface_mesh_simplification/Edge_collapse.h>

#include <CGAL/IO/Polyhedron_iostream.h>

#include <boost/property_map.hpp>
#include <CGAL/Unique_hash_map.h>

using namespace std ;
using namespace boost ;
using namespace CGAL ;

typedef Simple_cartesian<double> Kernel;
typedef Kernel::Vector_3         Vector;
typedef Kernel::Point_3          Point;

typedef Polyhedron_3<Kernel> Surface; 
typedef Surface::Halfedge_handle Halfedge_handle ;

using namespace CGAL::Triangulated_surface_mesh::Simplification::Edge_collapse ;

int main( int argc, char** argv ) 
{
  Surface surface; 
  
  ifstream is(argv[1]) ;
  is >> surface ;

  // Extra pointer external map
  Unique_hash_map<Halfedge_handle,void*> edge2ptr ;
  for ( Surface::Halfedge_iterator hi = surface.halfedges_begin(); hi != surface.halfedges_end() ; ++ hi )
    edge2ptr[hi] = 0 ;

  //
  // Set_empty_collapse_data is used because both Edge_length_cost and Midpoint_placement 
  // compute the values on deman.
  //   
  int r = edge_collapse(surface
                       ,Count_ratio_stop_condition<Surface>(0.10)
                       ,boost::make_assoc_property_map(edge2ptr) 
                       
                       ,Vertex_is_fixed_map_always_false<Surface>() // Same as default.
                       
                       ,Set_empty_collapse_data         <Surface>()
                       ,Edge_length_cost                <Surface>()
                       ,Midpoint_placement              <Surface>()
                       );

  cout << "\nFinished...\n" << r << " edges removed.\n"  << (surface.size_of_halfedges()/2) << " final edges.\n" ;
        
  ofstream os( argc > 2 ? argv[2] : "out.off" ) ;
  os << surface ;
  
  return 0 ;      
}

// EOF //
