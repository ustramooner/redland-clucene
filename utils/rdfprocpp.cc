// C++ raptor test program

#include <iostream>
#include <string>

#include <Redland.hpp>

using namespace std;

using namespace Redland;


int main(int argc, char *argv[])
{
  World red;

#if 0  
  cout << "Initialised Raptor " + r.getVersionString() << endl <<
    r.getShortCopyrightString() << endl;
  
  RaptorUri* uri = r.newUri("http://librdf.org/raptor/raptor.rdf");

  cout << "URI is " << uri << endl;

  std::vector<RaptorParserDescription> v=r.getParserDescriptions();
  for (unsigned int i = 0; i < v.size(); i++ ) {
    cout << "Parser " << i << endl << v[i] << endl;
  }

  if (r.isParserName("rdfxml")) {
    cout << "rdfxml IS a parser name\n";
  }

  if (!r.isParserName("foobar")) {
    cout << "foobar IS NOT a parser name\n";
  }

  RaptorParser* parser = r.newParser("rdfxml");

  cout << "Parser is " << parser->getName() << endl;

  try {
    parser->parseUri(uri, NULL);
  }
  catch (RaptorException &e) {
    cout << "parseUri(" << uri << ") failed with exception " << e.what() << endl;
  }
#endif
  
  return 0;
}