/*
---------------------------------------------------------------------------

    This file is part of uHAL.

    uHAL is a hardware access library and programming framework
    originally developed for upgrades of the Level-1 trigger of the CMS
    experiment at CERN.

    uHAL is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    uHAL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with uHAL.  If not, see <http://www.gnu.org/licenses/>.


      Andrew Rose, Imperial College, London
      email: awr01 <AT> imperial.ac.uk

      Marc Magrans de Abril, CERN
      email: marc.magrans.de.abril <AT> cern.ch

---------------------------------------------------------------------------
*/

#include "uhal/grammars/NodeTreeParametersGrammar.hpp"


#include <boost/spirit/include/qi.hpp>


namespace uhal
{
namespace grammars
{
  NodeTreeParametersGrammar::NodeTreeParametersGrammar() :
    NodeTreeParametersGrammar::base_type(query)
  {
    namespace qi = boost::spirit::qi;
    
    query =  pair >> *((qi::lit(';') | '&') >> pair);
    pair  =  key >> -('=' >> value);
    key   =  qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9");
    value = +qi::char_("a-zA-Z_0-9");
  }

}
}
