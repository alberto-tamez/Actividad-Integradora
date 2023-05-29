// Modificacion al ejemplo en: Boost.Polygon library voronoi_advanced_tutorial.cpp file

//          Copyright Andrii Sydorchuk 2010-2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org for updates, documentation, and revision history.

#include <iostream>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <string>

// This will work properly only with GCC compiler.
#include <limits>
#include <cmath>
#include <cstdint>
typedef long double fpt80;

// Random generators and distributions.
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include <boost/polygon/voronoi.hpp>
using namespace boost::polygon;

struct my_ulp_comparison
{
    enum Result
    {
        LESS = -1,
        EQUAL = 0,
        MORE = 1
    };

    Result operator()(fpt80 a, fpt80 b, unsigned int maxUlps) const
    {
        if (a == b)
            return EQUAL;
        if (std::isnan(a) || std::isnan(b))
            return LESS;

        if (a < 0.0 != b < 0.0)
        {
            if (a == 0 || std::nextafter(a, a - maxUlps) == 0)
                return EQUAL;
            return a > 0 ? MORE : LESS;
        }

        std::uint64_t aInt = *reinterpret_cast<std::uint64_t *>(&a);
        std::uint64_t bInt = *reinterpret_cast<std::uint64_t *>(&b);
        std::uint64_t ulpDelta = aInt > bInt ? aInt - bInt : bInt - aInt;

        if (ulpDelta <= maxUlps)
            return EQUAL;
        return a > b ? MORE : LESS;
    }
};

struct my_fpt_converter
{
    template <typename T>
    fpt80 operator()(const T &that) const
    {
        return static_cast<fpt80>(that);
    }

    template <std::size_t N>
    fpt80 operator()(const typename detail::extended_int<N> &that) const
    {
        fpt80 result = 0.0;
        for (std::size_t i = 1; i <= (std::min)((std::size_t)3, that.size()); ++i)
        {
            if (i != 1)
                result *= static_cast<fpt80>(0x100000000ULL);
            result += that.chunks()[that.size() - i];
        }
        return (that.count() < 0) ? -result : result;
    }
};

// Voronoi diagram traits.
struct my_voronoi_diagram_traits
{
    typedef fpt80 coordinate_type;
    typedef voronoi_cell<coordinate_type> cell_type;
    typedef voronoi_vertex<coordinate_type> vertex_type;
    typedef voronoi_edge<coordinate_type> edge_type;
    class vertex_equality_predicate_type
    {
    public:
        enum
        {
            ULPS = 128
        };
        bool operator()(const vertex_type &v1, const vertex_type &v2) const
        {
            return (ulp_cmp(v1.x(), v2.x(), ULPS) == my_ulp_comparison::EQUAL &&
                    ulp_cmp(v1.y(), v2.y(), ULPS) == my_ulp_comparison::EQUAL);
        }

    private:
        my_ulp_comparison ulp_cmp;
    };
};

// Voronoi ctype traits for 48-bit signed integer input coordinates.
struct my_voronoi_ctype_traits
{
    typedef boost::int64_t int_type;
    typedef detail::extended_int<3> int_x2_type;
    typedef detail::extended_int<3> uint_x2_type;
    typedef detail::extended_int<128> big_int_type;
    typedef fpt80 fpt_type;
    typedef fpt80 efpt_type;
    typedef my_ulp_comparison ulp_cmp_type;
    typedef my_fpt_converter to_fpt_converter_type;
    typedef my_fpt_converter to_efpt_converter_type;
};

const unsigned int GENERATED_POINTS = 10;
const boost::int64_t MAX = 0x1000000000000LL;


int main()
{
    unsigned int num_points;
    std::cin >> num_points;
    
    boost::int64_t x, y;
    voronoi_builder<boost::int64_t, my_voronoi_ctype_traits> vb;

    // Leer y agregar puntos al voronoi_builder
    for (unsigned int i = 0; i < num_points; ++i) {
        std::cin >> x >> y;
        vb.insert_point(x, y);
    }

    printf("Constructing Voronoi diagram of %d points...\n", GENERATED_POINTS);

    voronoi_diagram<fpt80, my_voronoi_diagram_traits> vd;

    vb.construct(&vd);

    printf("Resulting Voronoi graph has the following stats:\n");
    printf("Number of Voronoi cells: %lu.\n", vd.num_cells());
    printf("Number of Voronoi vertices: %lu.\n", vd.num_vertices());
    printf("Number of Voronoi edges: %lu.\n", vd.num_edges());

    std::cout << "Voronoi Edges:" << std::endl;
    for (auto it = vd.edges().begin(); it != vd.edges().end(); ++it)
    {
        if (it->is_finite())
        {
            std::cout << "  " << it->vertex0()->x() << "," << it->vertex0()->y()
                      << " - " << it->vertex1()->x() << "," << it->vertex1()->y() << std::endl;
        }
    }

    return 0;
}