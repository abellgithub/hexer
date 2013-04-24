/*****************************************************************************

    (c) 2013 Hobu, Inc. hobu.inc@gmail.com

    Author: Andrew Bell andrew.bell.ia at gmail.com

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the COPYING file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************************/

#include <hexer/Processor.hpp>
#include <hexer/OGR.hpp>
#include <hexer/hexer.hpp>
#include "las.hpp"


#include <fstream>
#include <sstream>

#include <boost/algorithm/string/compare.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

namespace po = boost::program_options;

using namespace std;


bool readHex(int& x, int& y, void* ctx)
{
    static int pos = 0;

    static int coords[] = {
        0, 0,
        1, 0,
        2, 1,
        3, 1,
        4, 2,
        4, 3,
        4, 4,
        3, 4,
        2, 5,
        1, 5,
        0, 5,
        -1, 4,
        -2, 4,
        -3, 3,
        -3, 2,
        -3, 1,
        -2, 1,
        -1, 0,
        -1, 2,
        0, 2,
        1, 3,
        2, 3
    };

    static int num_coords = sizeof(coords) / (sizeof(coords[0]));
    
    if (pos + 1 < num_coords) {
        x = coords[pos++];
        y = coords[pos++];
        return true;
    }
    return false;
}


std::string indent(int l)
{
    std::string tabs;

    tabs.append(l * 2, ' ');
    return tabs;
}

void dumpPath(hexer::Path *p)
{
    using namespace hexer;

    static int level = 0;
    Orientation o = p->orientation();
    std::string ostring = ((o == CLOCKWISE) ? "CLOCKWISE" : "ANTICLOCKWISE");
    indent(level);
    cerr << indent(level) << "Path length = " << p->pathLength() << "!\n";
    cerr << indent(level) << "Orientation = " << ostring << "!\n";
    vector<Path *> paths = p->subPaths();
    level++;
    for (int pi = 0; pi != paths.size(); ++pi)
    {
        dumpPath(paths[pi]);
    }
    level--;
}


void hextest(std::string filename)
{
    using namespace hexer;

    vector<GridInfo *> infos;
    GridInfo *gi = new GridInfo;

    gi->m_hexsize = 10;
    gi->m_density = 10;
    infos.push_back(gi);

    // LAS l(filename);
    // l.open();
    // process(infos, l.reader);
    
    processHexes(infos, readHex);

    // Dump hexes.
    for (HexIter iter = gi->begin(); iter != gi->end(); ++iter)
    {
        HexInfo hi = *iter;
        cerr << "Density/X/Y = " << hi.m_density << "/" <<
            hi.m_center.m_x << "/" << hi.m_center.m_y << "!\n";
    }

    delete gi;
}

void pathtest(std::string filename)
{
    using namespace hexer;

    vector<GridInfo *> infos;
    GridInfo *gi = new GridInfo;

    infos.push_back(gi);
    LAS l(filename);
    l.open();
    process(infos, l.reader);

    for (std::vector<Path*>::size_type pi = 0; pi < gi->rootPaths().size(); ++pi)
    {
        Path *p = gi->rootPaths()[pi];
        dumpPath(p);
    }

    delete gi;
}

enum FormatType
{
    Format_LAS = 0,
    Format_OGR = 1,
    Format_Unknown = 128

};

FormatType getDriver(std::string filename)
{
    std::string ext = boost::filesystem::extension(filename);
    if (boost::iequals(ext, ".LAS"))
        return Format_LAS;
    else
        return Format_OGR;
}

void boundary(std::string filename)
{
    using namespace hexer;

    vector<GridInfo *> infos;
    GridInfo *gi = new GridInfo;

    infos.push_back(gi);
    
    FormatType t = getDriver(filename);
    if (t == Format_LAS)
    {
        LAS l(filename);
        l.open();
        process(infos, l.reader);
    } else {
        OGR o(filename);
        o.open();
        process(infos, o.reader);
    }
    
 

    std::ostringstream multi;
    multi.setf(std::ios::fixed);
    multi.precision(8);
    
    gi->toWKT(multi);

    std::cout << multi.str() << std::endl;

    delete gi;
}



void OutputHelp( std::ostream & oss, po::options_description const& options)
{
    oss << "--------------------------------------------------------------------" << std::endl;
    oss << "    curse (" << GetFullVersion() << ")" << std::endl;
    oss << "--------------------------------------------------------------------" << std::endl;

    oss << options << std::endl;

    oss <<"For more information, see the full documentation for hexer at:" << std::endl;
    
    oss << " http://github.com/hobu/hexer" << std::endl;
    oss << "----------------------------------------------------------" << std::endl;

}


po::options_description getOptions()
{

    po::options_description basic = po::options_description("Basic");
    
    basic.add_options()
        ("help,h", po::value<bool>()->zero_tokens()->implicit_value(true), "This help message")
        // ("debug,d", po::value<bool>()->zero_tokens()->implicit_value(true), "Enable debug mode")
        // ("verbose,v", po::value<boost::uint32_t>()->default_value(0), "Set verbose message level")
        ("version", po::value<bool>()->zero_tokens()->implicit_value(true), "Show version info")
    ;

    po::options_description command = po::options_description("Command");
    
    command.add_options()
        ("input", po::value<std::string>(), "Input point set to curse")
        ("command", po::value<std::string>(), "Command to run on points ('boundary' or 'density')")
    ;

    po::options_description options;
    
    options.add(command).add(basic);
    return options;
    
}

int main(int argc, char* argv[])
{
    bool bVerbose(false);
    po::variables_map vm;    
    po::options_description options = getOptions();
    po::positional_options_description p;
    p.add("command", 1);
    p.add("input", 1);
     
    try
    {
        po::store(po::command_line_parser(argc, argv).
            options(options).positional(p).run(), vm);

        po::notify(vm);
    }     
    catch (boost::program_options::error e)
    {
        std::cout << "validation error: " << e.what() << std::endl;
        OutputHelp(std::cout, options);
        return 1;
    }
    
    if (vm.count("help")) 
    {
        OutputHelp(std::cout, options);
        return 1;
    }

    if (vm.count("version")) 
    {
        std::cout << GetFullVersion() << std::endl;
        return 0;
    }

    if (!vm.count("command") ) 
    {
        std::cerr << "Command not specified!\n";
        OutputHelp(std::cout, options);
        return 1;
    }
    if (!vm.count("input") ) 
    {
        std::cerr << "Input not specified!\n";
        OutputHelp(std::cout, options);
        return 1;
    } 

    std::string command = vm["command"].as<std::string>();
    std::string filename = vm["input"].as<std::string>();
    
	try
	{

		if (boost::iequals(command, "BOUNDARY"))
		{
			boundary(filename);
			return 0;
		}
    
		if (boost::iequals(command, "PATH"))
		{
			pathtest(filename);
			return 0;
		}

		if (boost::iequals(command, "HEX"))
		{
			hextest(filename);
			return 0;
		}    

		return 1;
	} catch (hexer::hexer_error const& e)
	{
		std::cout << "Curse failed with error: '" << e.what() << "'" << std::endl;
		return 1;
	}
    
}
