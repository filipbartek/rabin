// redirecter by Thomas Guest
// Source:
// http://wordaligned.org/articles/cpp-streambufs#tocredirect-streams
// Adjusted by Filip Bartek

#include <ostream>
#include <fstream>
#include "nullstream.hpp"

class redirecter
{
public:
    redirecter();
    redirecter(std::ostream & dst, std::ostream & src_new);
    ~redirecter();
    void set(std::ostream & dst, std::ostream & src_new);
private:
    std::ostream * src;
    std::streambuf * sbuf;
};

class optofstream : public std::ostream {
 public:
  optofstream(const char* filename, ios_base::openmode mode = ios_base::out);
  ~optofstream();
  bool active();
 private:
  std::ofstream ofs;
  null_stream ns;
  redirecter redirect;
};

class teebuf: public std::streambuf
{
public:
    // Construct a streambuf which tees output to both input
    // streambufs.
    teebuf(std::streambuf * sb1, std::streambuf * sb2)
        : sb1(sb1)
        , sb2(sb2)
    {
    }
private:
    // This tee buffer has no buffer. So every character "overflows"
    // and can be put directly into the teed buffers.
    virtual int overflow(int c)
    {
        if (c == EOF)
        {
            return !EOF;
        }
        else
        {
            int const r1 = sb1->sputc(c);
            int const r2 = sb2->sputc(c);
            return r1 == EOF || r2 == EOF ? EOF : c;
        }
    }

    // Sync both teed buffers.
    virtual int sync()
    {
        int const r1 = sb1->pubsync();
        int const r2 = sb2->pubsync();
        return r1 == 0 && r2 == 0 ? 0 : -1;
    }
private:
    std::streambuf * sb1;
    std::streambuf * sb2;
};

class teestream : public std::ostream
{
public:
    // Construct an ostream which tees output to the supplied
    // ostreams.
    teestream(std::ostream & o1, std::ostream & o2);
private:
    teebuf tbuf;
};
