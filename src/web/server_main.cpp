#include "server.hpp"

int main()
{
    ts::web::WebServer s;
    s.launch("127.0.0.1");
}
