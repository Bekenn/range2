#include <stdext/file.h>
#include <stdext/stream.h>
#include <stdext/unicode.h>


int main()
{
    stdext::file_input_stream infile(PATH_STR("UTF-8-test.txt"));
    stdext::file_output_stream outfile(PATH_STR("UTF-8-fixed.txt"));

    auto generator = stdext::stream_generator<char>(infile);
    auto consumer = stdext::stream_consumer<char>(outfile);

    generator >> stdext::to_utf16() >> stdext::to_utf8() >> consumer;
}
