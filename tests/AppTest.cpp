#include "app_helper.hpp"
#include <complex>
#include <cstdlib>

TEST_F(TApp, OneFlagShort) {
    app.add_flag("-c,--count");
    args = {"-c"};
    run();
    EXPECT_EQ(1u, app.count("-c"));
    EXPECT_EQ(1u, app.count("--count"));
}

TEST_F(TApp, OneFlagShortWindows) {
    app.add_flag("-c,--count");
    args = {"/c"};
    app.allow_windows_style_options();
    run();
    EXPECT_EQ(1u, app.count("-c"));
    EXPECT_EQ(1u, app.count("--count"));
}

TEST_F(TApp, WindowsLongShortMix1) {
    app.allow_windows_style_options();

    auto a = app.add_flag("-c");
    auto b = app.add_flag("--c");
    args = {"/c"};
    run();
    EXPECT_EQ(1u, a->count());
    EXPECT_EQ(0u, b->count());
}

TEST_F(TApp, WindowsLongShortMix2) {
    app.allow_windows_style_options();

    auto a = app.add_flag("--c");
    auto b = app.add_flag("-c");
    args = {"/c"};
    run();
    EXPECT_EQ(1u, a->count());
    EXPECT_EQ(0u, b->count());
}

TEST_F(TApp, CountNonExist) {
    app.add_flag("-c,--count");
    args = {"-c"};
    run();
    EXPECT_THROW(app.count("--nonexist"), CLI::OptionNotFound);
}

TEST_F(TApp, OneFlagLong) {
    app.add_flag("-c,--count");
    args = {"--count"};
    run();
    EXPECT_EQ(1u, app.count("-c"));
    EXPECT_EQ(1u, app.count("--count"));
}

TEST_F(TApp, DashedOptions) {
    app.add_flag("-c");
    app.add_flag("--q");
    app.add_flag("--this,--that");

    args = {"-c", "--q", "--this", "--that"};
    run();
    EXPECT_EQ(1u, app.count("-c"));
    EXPECT_EQ(1u, app.count("--q"));
    EXPECT_EQ(2u, app.count("--this"));
    EXPECT_EQ(2u, app.count("--that"));
}

TEST_F(TApp, DashedOptionsSingleString) {
    app.add_flag("-c");
    app.add_flag("--q");
    app.add_flag("--this,--that");

    app.parse("-c --q --this --that");
    EXPECT_EQ(1u, app.count("-c"));
    EXPECT_EQ(1u, app.count("--q"));
    EXPECT_EQ(2u, app.count("--this"));
    EXPECT_EQ(2u, app.count("--that"));
}

TEST_F(TApp, OneFlagRef) {
    int ref;
    app.add_flag("-c,--count", ref);
    args = {"--count"};
    run();
    EXPECT_EQ(1u, app.count("-c"));
    EXPECT_EQ(1u, app.count("--count"));
    EXPECT_EQ(1, ref);
}

TEST_F(TApp, OneFlagRefValue) {
    int ref;
    app.add_flag("-c,--count", ref);
    args = {"--count=7"};
    run();
    EXPECT_EQ(1u, app.count("-c"));
    EXPECT_EQ(1u, app.count("--count"));
    EXPECT_EQ(7, ref);
}

TEST_F(TApp, OneFlagRefValueFalse) {
    int ref;
    app.add_flag("-c,--count", ref);
    args = {"--count=false"};
    run();
    EXPECT_EQ(1u, app.count("-c"));
    EXPECT_EQ(1u, app.count("--count"));
    EXPECT_EQ(-1, ref);

    args = {"--count=0"};
    run();
    EXPECT_EQ(1u, app.count("-c"));
    EXPECT_EQ(1u, app.count("--count"));
    EXPECT_EQ(-1, ref);

    args = {"--count=happy"};
    EXPECT_THROW(run(), CLI::ConversionError);
}

TEST_F(TApp, FlagNegation) {
    int ref;
    app.add_flag("-c,--count,--ncount{false}", ref);
    args = {"--count", "-c", "--ncount"};
    run();
    EXPECT_EQ(3u, app.count("-c"));
    EXPECT_EQ(3u, app.count("--count"));
    EXPECT_EQ(3u, app.count("--ncount"));
    EXPECT_EQ(1, ref);
}

TEST_F(TApp, FlagNegationShortcutNotation) {
    int ref;
    app.add_flag("-c,--count,!--ncount", ref);
    args = {"--count", "-c", "--ncount"};
    run();
    EXPECT_EQ(3u, app.count("-c"));
    EXPECT_EQ(3u, app.count("--count"));
    EXPECT_EQ(3u, app.count("--ncount"));
    EXPECT_EQ(1, ref);
}

TEST_F(TApp, OneString) {
    std::string str;
    app.add_option("-s,--string", str);
    args = {"--string", "mystring"};
    run();
    EXPECT_EQ(1u, app.count("-s"));
    EXPECT_EQ(1u, app.count("--string"));
    EXPECT_EQ(str, "mystring");
}

TEST_F(TApp, OneStringWindowsStyle) {
    std::string str;
    app.add_option("-s,--string", str);
    args = {"/string", "mystring"};
    app.allow_windows_style_options();
    run();
    EXPECT_EQ(1u, app.count("-s"));
    EXPECT_EQ(1u, app.count("--string"));
    EXPECT_EQ(str, "mystring");
}

TEST_F(TApp, OneStringSingleStringInput) {
    std::string str;
    app.add_option("-s,--string", str);

    app.parse("--string mystring");
    EXPECT_EQ(1u, app.count("-s"));
    EXPECT_EQ(1u, app.count("--string"));
    EXPECT_EQ(str, "mystring");
}

TEST_F(TApp, OneStringEqualVersion) {
    std::string str;
    app.add_option("-s,--string", str);
    args = {"--string=mystring"};
    run();
    EXPECT_EQ(1u, app.count("-s"));
    EXPECT_EQ(1u, app.count("--string"));
    EXPECT_EQ(str, "mystring");
}

TEST_F(TApp, OneStringEqualVersionWindowsStyle) {
    std::string str;
    app.add_option("-s,--string", str);
    args = {"/string:mystring"};
    app.allow_windows_style_options();
    run();
    EXPECT_EQ(1u, app.count("-s"));
    EXPECT_EQ(1u, app.count("--string"));
    EXPECT_EQ(str, "mystring");
}

TEST_F(TApp, OneStringEqualVersionSingleString) {
    std::string str;
    app.add_option("-s,--string", str);
    app.parse("--string=mystring");
    EXPECT_EQ(1u, app.count("-s"));
    EXPECT_EQ(1u, app.count("--string"));
    EXPECT_EQ(str, "mystring");
}

TEST_F(TApp, OneStringEqualVersionSingleStringQuoted) {
    std::string str;
    app.add_option("-s,--string", str);
    app.parse("--string=\"this is my quoted string\"");
    EXPECT_EQ(1u, app.count("-s"));
    EXPECT_EQ(1u, app.count("--string"));
    EXPECT_EQ(str, "this is my quoted string");
}

TEST_F(TApp, OneStringEqualVersionSingleStringQuotedMultiple) {
    std::string str, str2, str3;
    app.add_option("-s,--string", str);
    app.add_option("-t,--tstr", str2);
    app.add_option("-m,--mstr", str3);
    app.parse("--string=\"this is my quoted string\" -t 'qstring 2' -m=`\"quoted string\"`");
    EXPECT_EQ(str, "this is my quoted string");
    EXPECT_EQ(str2, "qstring 2");
    EXPECT_EQ(str3, "\"quoted string\"");
}

TEST_F(TApp, OneStringEqualVersionSingleStringEmbeddedEqual) {
    std::string str, str2, str3;
    app.add_option("-s,--string", str);
    app.add_option("-t,--tstr", str2);
    app.add_option("-m,--mstr", str3);
    app.parse("--string=\"app=\\\"test1 b\\\" test2=\\\"frogs\\\"\" -t 'qstring 2' -m=`\"quoted string\"`");
    EXPECT_EQ(str, "app=\"test1 b\" test2=\"frogs\"");
    EXPECT_EQ(str2, "qstring 2");
    EXPECT_EQ(str3, "\"quoted string\"");

    app.parse("--string=\"app='test1 b' test2='frogs'\" -t 'qstring 2' -m=`\"quoted string\"`");
    EXPECT_EQ(str, "app='test1 b' test2='frogs'");
    EXPECT_EQ(str2, "qstring 2");
    EXPECT_EQ(str3, "\"quoted string\"");
}

TEST_F(TApp, OneStringEqualVersionSingleStringEmbeddedEqualWindowsStyle) {
    std::string str, str2, str3;
    app.add_option("-s,--string", str);
    app.add_option("-t,--tstr", str2);
    app.add_option("--mstr", str3);
    app.allow_windows_style_options();
    app.parse("/string:\"app:\\\"test1 b\\\" test2:\\\"frogs\\\"\" /t 'qstring 2' /mstr:`\"quoted string\"`");
    EXPECT_EQ(str, "app:\"test1 b\" test2:\"frogs\"");
    EXPECT_EQ(str2, "qstring 2");
    EXPECT_EQ(str3, "\"quoted string\"");

    app.parse("/string:\"app:'test1 b' test2:'frogs'\" /t 'qstring 2' /mstr:`\"quoted string\"`");
    EXPECT_EQ(str, "app:'test1 b' test2:'frogs'");
    EXPECT_EQ(str2, "qstring 2");
    EXPECT_EQ(str3, "\"quoted string\"");
}

TEST_F(TApp, OneStringEqualVersionSingleStringQuotedMultipleMixedStyle) {
    std::string str, str2, str3;
    app.add_option("-s,--string", str);
    app.add_option("-t,--tstr", str2);
    app.add_option("-m,--mstr", str3);
    app.allow_windows_style_options();
    app.parse("/string:\"this is my quoted string\" /t 'qstring 2' -m=`\"quoted string\"`");
    EXPECT_EQ(str, "this is my quoted string");
    EXPECT_EQ(str2, "qstring 2");
    EXPECT_EQ(str3, "\"quoted string\"");
}

TEST_F(TApp, OneStringEqualVersionSingleStringQuotedMultipleInMiddle) {
    std::string str, str2, str3;
    app.add_option("-s,--string", str);
    app.add_option("-t,--tstr", str2);
    app.add_option("-m,--mstr", str3);
    app.parse(R"raw(--string="this is my quoted string" -t "qst\"ring 2" -m=`"quoted string"`")raw");
    EXPECT_EQ(str, "this is my quoted string");
    EXPECT_EQ(str2, "qst\"ring 2");
    EXPECT_EQ(str3, "\"quoted string\"");
}

TEST_F(TApp, OneStringEqualVersionSingleStringQuotedEscapedCharacters) {
    std::string str, str2, str3;
    app.add_option("-s,--string", str);
    app.add_option("-t,--tstr", str2);
    app.add_option("-m,--mstr", str3);
    app.parse(R"raw(--string="this is my \"quoted\" string" -t 'qst\'ring 2' -m=`"quoted\` string"`")raw");
    EXPECT_EQ(str, "this is my \"quoted\" string");
    EXPECT_EQ(str2, "qst\'ring 2");
    EXPECT_EQ(str3, "\"quoted` string\"");
}

TEST_F(TApp, OneStringEqualVersionSingleStringQuotedMultipleWithEqual) {
    std::string str, str2, str3, str4;
    app.add_option("-s,--string", str);
    app.add_option("-t,--tstr", str2);
    app.add_option("-m,--mstr", str3);
    app.add_option("-j,--jstr", str4);
    app.parse("--string=\"this is my quoted string\" -t 'qstring 2' -m=`\"quoted string\"` --jstr=Unquoted");
    EXPECT_EQ(str, "this is my quoted string");
    EXPECT_EQ(str2, "qstring 2");
    EXPECT_EQ(str3, "\"quoted string\"");
    EXPECT_EQ(str4, "Unquoted");
}

TEST_F(TApp, OneStringEqualVersionSingleStringQuotedMultipleWithEqualAndProgram) {
    std::string str, str2, str3, str4;
    app.add_option("-s,--string", str);
    app.add_option("-t,--tstr", str2);
    app.add_option("-m,--mstr", str3);
    app.add_option("-j,--jstr", str4);
    app.parse("program --string=\"this is my quoted string\" -t 'qstring 2' -m=`\"quoted string\"` --jstr=Unquoted",
              true);
    EXPECT_EQ(str, "this is my quoted string");
    EXPECT_EQ(str2, "qstring 2");
    EXPECT_EQ(str3, "\"quoted string\"");
    EXPECT_EQ(str4, "Unquoted");
}

TEST_F(TApp, TogetherInt) {
    int i;
    app.add_option("-i,--int", i);
    args = {"-i4"};
    run();
    EXPECT_EQ(1u, app.count("--int"));
    EXPECT_EQ(1u, app.count("-i"));
    EXPECT_EQ(i, 4);
}

TEST_F(TApp, SepInt) {
    int i;
    app.add_option("-i,--int", i);
    args = {"-i", "4"};
    run();
    EXPECT_EQ(1u, app.count("--int"));
    EXPECT_EQ(1u, app.count("-i"));
    EXPECT_EQ(i, 4);
}

TEST_F(TApp, OneStringAgain) {
    std::string str;
    app.add_option("-s,--string", str);
    args = {"--string", "mystring"};
    run();
    EXPECT_EQ(1u, app.count("-s"));
    EXPECT_EQ(1u, app.count("--string"));
    EXPECT_EQ(str, "mystring");
}

TEST_F(TApp, OneStringFunction) {
    std::string str;
    app.add_option_function<std::string>("-s,--string", [&str](const std::string &val) {
        str = val;
        return true;
    });
    args = {"--string", "mystring"};
    run();
    EXPECT_EQ(1u, app.count("-s"));
    EXPECT_EQ(1u, app.count("--string"));
    EXPECT_EQ(str, "mystring");
}

TEST_F(TApp, doubleFunction) {
    double res;
    app.add_option_function<double>("--val", [&res](double val) {
        res = std::abs(val + 54);
        return true;
    });
    args = {"--val", "-354.356"};
    run();
    EXPECT_EQ(res, 300.356);
}

TEST_F(TApp, doubleFunctionFail) {
    double res;
    app.add_option_function<double>("--val", [&res](double val) {
        res = std::abs(val + 54);
        return true;
    });
    args = {"--val", "not_double"};
    EXPECT_THROW(run(), CLI::ConversionError);
}

TEST_F(TApp, doubleVectorFunction) {
    std::vector<double> res;
    app.add_option_function<std::vector<double>>("--val", [&res](const std::vector<double> &val) {
        res = val;
        std::transform(res.begin(), res.end(), res.begin(), [](double v) { return v + 5.0; });
        return true;
    });
    args = {"--val", "5", "--val", "6", "--val", "7"};
    run();
    EXPECT_EQ(res.size(), 3u);
    EXPECT_EQ(res[0], 10.0);
    EXPECT_EQ(res[2], 12.0);
}

TEST_F(TApp, doubleVectorFunctionFail) {
    std::vector<double> res;
    app.add_option_function<std::vector<double>>("--val", [&res](const std::vector<double> &val) {
        res = val;
        std::transform(res.begin(), res.end(), res.begin(), [](double v) { return v + 5.0; });
        return true;
    });
    args = {"--val", "five", "--val", "nine", "--val", "7"};
    EXPECT_THROW(run(), CLI::ConversionError);
}

TEST_F(TApp, DefaultStringAgain) {
    std::string str = "previous";
    app.add_option("-s,--string", str);
    run();
    EXPECT_EQ(0u, app.count("-s"));
    EXPECT_EQ(0u, app.count("--string"));
    EXPECT_EQ(str, "previous");
}

TEST_F(TApp, DefaultStringAgainEmpty) {
    std::string str = "previous";
    app.add_option("-s,--string", str);
    app.parse("   ");
    EXPECT_EQ(0u, app.count("-s"));
    EXPECT_EQ(0u, app.count("--string"));
    EXPECT_EQ(str, "previous");
}

TEST_F(TApp, DualOptions) {

    std::string str = "previous";
    std::vector<std::string> vstr = {"previous"};
    std::vector<std::string> ans = {"one", "two"};
    app.add_option("-s,--string", str);
    app.add_option("-v,--vector", vstr);

    args = {"--vector=one", "--vector=two"};
    run();
    EXPECT_EQ(ans, vstr);

    args = {"--string=one", "--string=two"};
    EXPECT_THROW(run(), CLI::ArgumentMismatch);
}

TEST_F(TApp, LotsOfFlags) {

    app.add_flag("-a");
    app.add_flag("-A");
    app.add_flag("-b");

    args = {"-a", "-b", "-aA"};
    run();
    EXPECT_EQ(2u, app.count("-a"));
    EXPECT_EQ(1u, app.count("-b"));
    EXPECT_EQ(1u, app.count("-A"));
}

TEST_F(TApp, LotsOfFlagsSingleString) {

    app.add_flag("-a");
    app.add_flag("-A");
    app.add_flag("-b");

    app.parse("-a -b -aA");
    EXPECT_EQ(2u, app.count("-a"));
    EXPECT_EQ(1u, app.count("-b"));
    EXPECT_EQ(1u, app.count("-A"));
}

TEST_F(TApp, LotsOfFlagsSingleStringExtraSpace) {

    app.add_flag("-a");
    app.add_flag("-A");
    app.add_flag("-b");

    app.parse("  -a    -b    -aA   ");
    EXPECT_EQ(2u, app.count("-a"));
    EXPECT_EQ(1u, app.count("-b"));
    EXPECT_EQ(1u, app.count("-A"));
}

TEST_F(TApp, BoolAndIntFlags) {

    bool bflag;
    int iflag;
    unsigned int uflag;

    app.add_flag("-b", bflag);
    app.add_flag("-i", iflag);
    app.add_flag("-u", uflag);

    args = {"-b", "-i", "-u"};
    run();
    EXPECT_TRUE(bflag);
    EXPECT_EQ(1, iflag);
    EXPECT_EQ((unsigned int)1, uflag);

    args = {"-b", "-b"};
    ASSERT_NO_THROW(run());
    EXPECT_TRUE(bflag);

    bflag = false;

    args = {"-iiiuu"};
    run();
    EXPECT_FALSE(bflag);
    EXPECT_EQ(3, iflag);
    EXPECT_EQ((unsigned int)2, uflag);
}

TEST_F(TApp, BoolOnlyFlag) {
    bool bflag;
    app.add_flag("-b", bflag)->multi_option_policy(CLI::MultiOptionPolicy::Throw);

    args = {"-b"};
    ASSERT_NO_THROW(run());
    EXPECT_TRUE(bflag);

    args = {"-b", "-b"};
    EXPECT_THROW(run(), CLI::ConversionError);
}

TEST_F(TApp, BoolOption) {
    bool bflag;
    app.add_option("-b", bflag);

    args = {"-b", "false"};
    run();
    EXPECT_FALSE(bflag);

    args = {"-b", "1"};
    run();
    EXPECT_TRUE(bflag);

    args = {"-b", "-7"};
    run();
    EXPECT_FALSE(bflag);
}

TEST_F(TApp, ShortOpts) {

    unsigned long long funnyint;
    std::string someopt;
    app.add_flag("-z", funnyint);
    app.add_option("-y", someopt);

    args = {
        "-zzyzyz",
    };

    run();

    EXPECT_EQ(2u, app.count("-z"));
    EXPECT_EQ(1u, app.count("-y"));
    EXPECT_EQ((unsigned long long)2, funnyint);
    EXPECT_EQ("zyz", someopt);
}

TEST_F(TApp, DefaultOpts) {

    int i = 3;
    std::string s = "HI";

    app.add_option("-i,i", i, "", false);
    app.add_option("-s,s", s, "", true);

    args = {"-i2", "9"};

    run();

    EXPECT_EQ(1u, app.count("i"));
    EXPECT_EQ(1u, app.count("-s"));
    EXPECT_EQ(2, i);
    EXPECT_EQ("9", s);
}

TEST_F(TApp, TakeLastOpt) {

    std::string str;
    app.add_option("--str", str)->multi_option_policy(CLI::MultiOptionPolicy::TakeLast);

    args = {"--str=one", "--str=two"};

    run();

    EXPECT_EQ(str, "two");
}

TEST_F(TApp, TakeLastOpt2) {

    std::string str;
    app.add_option("--str", str)->take_last();

    args = {"--str=one", "--str=two"};

    run();

    EXPECT_EQ(str, "two");
}

TEST_F(TApp, TakeFirstOpt) {

    std::string str;
    app.add_option("--str", str)->multi_option_policy(CLI::MultiOptionPolicy::TakeFirst);

    args = {"--str=one", "--str=two"};

    run();

    EXPECT_EQ(str, "one");
}

TEST_F(TApp, TakeFirstOpt2) {

    std::string str;
    app.add_option("--str", str)->take_first();

    args = {"--str=one", "--str=two"};

    run();

    EXPECT_EQ(str, "one");
}

TEST_F(TApp, JoinOpt) {

    std::string str;
    app.add_option("--str", str)->multi_option_policy(CLI::MultiOptionPolicy::Join);

    args = {"--str=one", "--str=two"};

    run();

    EXPECT_EQ(str, "one\ntwo");
}

TEST_F(TApp, JoinOpt2) {

    std::string str;
    app.add_option("--str", str)->join();

    args = {"--str=one", "--str=two"};

    run();

    EXPECT_EQ(str, "one\ntwo");
}

TEST_F(TApp, TakeLastOptMulti) {
    std::vector<int> vals;
    app.add_option("--long", vals)->expected(2)->take_last();

    args = {"--long", "1", "2", "3"};

    run();

    EXPECT_EQ(vals, std::vector<int>({2, 3}));
}

TEST_F(TApp, TakeFirstOptMulti) {
    std::vector<int> vals;
    app.add_option("--long", vals)->expected(2)->take_first();

    args = {"--long", "1", "2", "3"};

    run();

    EXPECT_EQ(vals, std::vector<int>({1, 2}));
}

TEST_F(TApp, ComplexOptMulti) {
    std::complex<double> val;
    app.add_complex("--long", val)->take_first();

    args = {"--long", "1", "2", "3", "4"};

    run();

    EXPECT_DOUBLE_EQ(val.real(), 1);
    EXPECT_DOUBLE_EQ(val.imag(), 2);
}

TEST_F(TApp, MissingValueNonRequiredOpt) {
    int count;
    app.add_option("-c,--count", count);

    args = {"-c"};
    EXPECT_THROW(run(), CLI::ArgumentMismatch);

    args = {"--count"};
    EXPECT_THROW(run(), CLI::ArgumentMismatch);
}

TEST_F(TApp, MissingValueMoreThan) {
    std::vector<int> vals1;
    std::vector<int> vals2;
    app.add_option("-v", vals1)->expected(-2);
    app.add_option("--vals", vals2)->expected(-2);

    args = {"-v", "2"};
    EXPECT_THROW(run(), CLI::ArgumentMismatch);

    args = {"--vals", "4"};
    EXPECT_THROW(run(), CLI::ArgumentMismatch);
}

TEST_F(TApp, NoMissingValueMoreThan) {
    std::vector<int> vals1;
    std::vector<int> vals2;
    app.add_option("-v", vals1)->expected(-2);
    app.add_option("--vals", vals2)->expected(-2);

    args = {"-v", "2", "3", "4"};
    run();
    EXPECT_EQ(vals1, std::vector<int>({2, 3, 4}));

    args = {"--vals", "2", "3", "4"};
    run();
    EXPECT_EQ(vals2, std::vector<int>({2, 3, 4}));
}

TEST_F(TApp, NotRequiredOptsSingle) {

    std::string str;
    app.add_option("--str", str);

    args = {"--str"};

    EXPECT_THROW(run(), CLI::ArgumentMismatch);
}

TEST_F(TApp, NotRequiredOptsSingleShort) {

    std::string str;
    app.add_option("-s", str);

    args = {"-s"};

    EXPECT_THROW(run(), CLI::ArgumentMismatch);
}

TEST_F(TApp, RequiredOptsSingle) {

    std::string str;
    app.add_option("--str", str)->required();

    args = {"--str"};

    EXPECT_THROW(run(), CLI::ArgumentMismatch);
}

TEST_F(TApp, RequiredOptsSingleShort) {

    std::string str;
    app.add_option("-s", str)->required();

    args = {"-s"};

    EXPECT_THROW(run(), CLI::ArgumentMismatch);
}

TEST_F(TApp, RequiredOptsDouble) {

    std::vector<std::string> strs;
    app.add_option("--str", strs)->required()->expected(2);

    args = {"--str", "one"};

    EXPECT_THROW(run(), CLI::ArgumentMismatch);

    args = {"--str", "one", "two"};

    run();

    EXPECT_EQ(strs, std::vector<std::string>({"one", "two"}));
}

TEST_F(TApp, RequiredOptsDoubleShort) {

    std::vector<std::string> strs;
    app.add_option("-s", strs)->required()->expected(2);

    args = {"-s", "one"};

    EXPECT_THROW(run(), CLI::ArgumentMismatch);

    args = {"-s", "one", "-s", "one", "-s", "one"};

    EXPECT_THROW(run(), CLI::ArgumentMismatch);
}

TEST_F(TApp, RequiredOptsDoubleNeg) {
    std::vector<std::string> strs;
    app.add_option("-s", strs)->required()->expected(-2);

    args = {"-s", "one"};

    EXPECT_THROW(run(), CLI::ArgumentMismatch);

    args = {"-s", "one", "two", "-s", "three"};

    ASSERT_NO_THROW(run());
    EXPECT_EQ(strs, std::vector<std::string>({"one", "two", "three"}));

    args = {"-s", "one", "two"};
    ASSERT_NO_THROW(run());
    EXPECT_EQ(strs, std::vector<std::string>({"one", "two"}));
}

// This makes sure unlimited option priority is
// correct for space vs. no space #90
TEST_F(TApp, PositionalNoSpace) {
    std::vector<std::string> options;
    std::string foo, bar;

    app.add_option("-O", options);
    app.add_option("foo", foo)->required();
    app.add_option("bar", bar)->required();

    args = {"-O", "Test", "param1", "param2"};
    run();

    EXPECT_EQ(options.size(), 1u);
    EXPECT_EQ(options.at(0), "Test");

    args = {"-OTest", "param1", "param2"};
    run();

    EXPECT_EQ(options.size(), 1u);
    EXPECT_EQ(options.at(0), "Test");
}

// Tests positionals at end
TEST_F(TApp, PositionalAtEnd) {
    std::string options;
    std::string foo;

    app.add_option("-O", options);
    app.add_option("foo", foo);
    app.positionals_at_end();
    EXPECT_TRUE(app.get_positionals_at_end());
    args = {"-O", "Test", "param1"};
    run();

    EXPECT_EQ(options, "Test");
    EXPECT_EQ(foo, "param1");

    args = {"param2", "-O", "Test"};
    EXPECT_THROW(run(), CLI::ExtrasError);
}

TEST_F(TApp, PositionalNoSpaceLong) {
    std::vector<std::string> options;
    std::string foo, bar;

    app.add_option("--option", options);
    app.add_option("foo", foo)->required();
    app.add_option("bar", bar)->required();

    args = {"--option", "Test", "param1", "param2"};
    run();

    EXPECT_EQ(options.size(), 1u);
    EXPECT_EQ(options.at(0), "Test");

    args = {"--option=Test", "param1", "param2"};
    run();

    EXPECT_EQ(options.size(), 1u);
    EXPECT_EQ(options.at(0), "Test");
}

TEST_F(TApp, RequiredOptsUnlimited) {

    std::vector<std::string> strs;
    app.add_option("--str", strs)->required();

    args = {"--str"};
    EXPECT_THROW(run(), CLI::ArgumentMismatch);

    args = {"--str", "one", "--str", "two"};
    run();
    EXPECT_EQ(strs, std::vector<std::string>({"one", "two"}));

    args = {"--str", "one", "two"};
    run();
    EXPECT_EQ(strs, std::vector<std::string>({"one", "two"}));

    // It's better to feed a hungry option than to feed allow_extras
    app.allow_extras();
    run();
    EXPECT_EQ(strs, std::vector<std::string>({"one", "two"}));
    EXPECT_EQ(app.remaining(), std::vector<std::string>({}));

    app.allow_extras(false);
    std::vector<std::string> remain;
    app.add_option("positional", remain);
    run();
    EXPECT_EQ(strs, std::vector<std::string>({"one", "two"}));
    EXPECT_EQ(remain, std::vector<std::string>());

    args = {"--str", "one", "--", "two"};

    run();
    EXPECT_EQ(strs, std::vector<std::string>({"one"}));
    EXPECT_EQ(remain, std::vector<std::string>({"two"}));

    args = {"one", "--str", "two"};

    run();
    EXPECT_EQ(strs, std::vector<std::string>({"two"}));
    EXPECT_EQ(remain, std::vector<std::string>({"one"}));
}

TEST_F(TApp, RequiredOptsUnlimitedShort) {

    std::vector<std::string> strs;
    app.add_option("-s", strs)->required();

    args = {"-s"};
    EXPECT_THROW(run(), CLI::ArgumentMismatch);

    args = {"-s", "one", "-s", "two"};
    run();
    EXPECT_EQ(strs, std::vector<std::string>({"one", "two"}));

    args = {"-s", "one", "two"};
    run();
    EXPECT_EQ(strs, std::vector<std::string>({"one", "two"}));

    // It's better to feed a hungry option than to feed allow_extras
    app.allow_extras();
    run();
    EXPECT_EQ(strs, std::vector<std::string>({"one", "two"}));
    EXPECT_EQ(app.remaining(), std::vector<std::string>({}));

    app.allow_extras(false);
    std::vector<std::string> remain;
    app.add_option("positional", remain);
    run();
    EXPECT_EQ(strs, std::vector<std::string>({"one", "two"}));
    EXPECT_EQ(remain, std::vector<std::string>());

    args = {"-s", "one", "--", "two"};

    run();
    EXPECT_EQ(strs, std::vector<std::string>({"one"}));
    EXPECT_EQ(remain, std::vector<std::string>({"two"}));

    args = {"one", "-s", "two"};

    run();
    EXPECT_EQ(strs, std::vector<std::string>({"two"}));
    EXPECT_EQ(remain, std::vector<std::string>({"one"}));
}

TEST_F(TApp, OptsUnlimitedEnd) {
    std::vector<std::string> strs;
    app.add_option("-s,--str", strs);
    app.allow_extras();

    args = {"one", "-s", "two", "three", "--", "four"};

    run();

    EXPECT_EQ(strs, std::vector<std::string>({"two", "three"}));
    EXPECT_EQ(app.remaining(), std::vector<std::string>({"one", "four"}));
}

TEST_F(TApp, RequireOptPriority) {

    std::vector<std::string> strs;
    app.add_option("--str", strs)->required();
    std::vector<std::string> remain;
    app.add_option("positional", remain)->expected(2);

    args = {"--str", "one", "two", "three"};
    run();

    EXPECT_EQ(strs, std::vector<std::string>({"one"}));
    EXPECT_EQ(remain, std::vector<std::string>({"two", "three"}));

    args = {"two", "three", "--str", "one", "four"};
    run();

    EXPECT_EQ(strs, std::vector<std::string>({"one", "four"}));
    EXPECT_EQ(remain, std::vector<std::string>({"two", "three"}));
}

TEST_F(TApp, RequireOptPriorityShort) {

    std::vector<std::string> strs;
    app.add_option("-s", strs)->required();
    std::vector<std::string> remain;
    app.add_option("positional", remain)->expected(2);

    args = {"-s", "one", "two", "three"};
    run();

    EXPECT_EQ(strs, std::vector<std::string>({"one"}));
    EXPECT_EQ(remain, std::vector<std::string>({"two", "three"}));

    args = {"two", "three", "-s", "one", "four"};
    run();

    EXPECT_EQ(strs, std::vector<std::string>({"one", "four"}));
    EXPECT_EQ(remain, std::vector<std::string>({"two", "three"}));
}

TEST_F(TApp, NotRequiedExpectedDouble) {

    std::vector<std::string> strs;
    app.add_option("--str", strs)->expected(2);

    args = {"--str", "one"};

    EXPECT_THROW(run(), CLI::ArgumentMismatch);
}

TEST_F(TApp, NotRequiedExpectedDoubleShort) {

    std::vector<std::string> strs;
    app.add_option("-s", strs)->expected(2);

    args = {"-s", "one"};

    EXPECT_THROW(run(), CLI::ArgumentMismatch);
}

TEST_F(TApp, RequiredFlags) {
    app.add_flag("-a")->required();
    app.add_flag("-b")->mandatory(); // Alternate term

    EXPECT_THROW(run(), CLI::RequiredError);

    args = {"-a"};
    EXPECT_THROW(run(), CLI::RequiredError);

    args = {"-b"};
    EXPECT_THROW(run(), CLI::RequiredError);

    args = {"-a", "-b"};
    run();
}

TEST_F(TApp, CallbackFlags) {

    size_t value = 0;

    auto func = [&value](size_t x) { value = x; };

    app.add_flag_function("-v", func);

    run();
    EXPECT_EQ(value, 0u);

    args = {"-v"};
    run();
    EXPECT_EQ(value, 1u);

    args = {"-vv"};
    run();
    EXPECT_EQ(value, 2u);

    EXPECT_THROW(app.add_flag_function("hi", func), CLI::IncorrectConstruction);
}

TEST_F(TApp, CallbackFlagsFalse) {
    int value = 0;

    auto func = [&value](int x) { value = x; };

    app.add_flag_function("-v,-f{false},--val,--fval{false}", func);

    run();
    EXPECT_EQ(value, 0u);

    args = {"-f"};
    run();
    EXPECT_EQ(value, -1);

    args = {"-vfv"};
    run();
    EXPECT_EQ(value, 1);

    args = {"--fval"};
    run();
    EXPECT_EQ(value, -1);

    args = {"--fval=2"};
    run();
    EXPECT_EQ(value, -2);

    EXPECT_THROW(app.add_flag_function("hi", func), CLI::IncorrectConstruction);
}

TEST_F(TApp, CallbackFlagsFalseShortcut) {
    int value = 0;

    auto func = [&value](int x) { value = x; };

    app.add_flag_function("-v,!-f,--val,!--fval", func);

    run();
    EXPECT_EQ(value, 0u);

    args = {"-f"};
    run();
    EXPECT_EQ(value, -1);

    args = {"-vfv"};
    run();
    EXPECT_EQ(value, 1);

    args = {"--fval"};
    run();
    EXPECT_EQ(value, -1);

    args = {"--fval=2"};
    run();
    EXPECT_EQ(value, -2);

    EXPECT_THROW(app.add_flag_function("hi", func), CLI::IncorrectConstruction);
}

#if __cplusplus >= 201402L || _MSC_VER >= 1900
TEST_F(TApp, CallbackFlagsAuto) {

    size_t value = 0;

    auto func = [&value](size_t x) { value = x; };

    app.add_flag("-v", func);

    run();
    EXPECT_EQ(value, 0u);

    args = {"-v"};
    run();
    EXPECT_EQ(value, 1u);

    args = {"-vv"};
    run();
    EXPECT_EQ(value, 2u);

    EXPECT_THROW(app.add_flag("hi", func), CLI::IncorrectConstruction);
}
#endif

TEST_F(TApp, Positionals) {

    std::string posit1;
    std::string posit2;
    app.add_option("posit1", posit1);
    app.add_option("posit2", posit2);

    args = {"thing1", "thing2"};

    run();

    EXPECT_EQ(1u, app.count("posit1"));
    EXPECT_EQ(1u, app.count("posit2"));
    EXPECT_EQ("thing1", posit1);
    EXPECT_EQ("thing2", posit2);
}

TEST_F(TApp, ForcedPositional) {
    std::vector<std::string> posit;
    auto one = app.add_flag("--one");
    app.add_option("posit", posit);

    args = {"--one", "two", "three"};
    run();
    std::vector<std::string> answers1 = {"two", "three"};
    EXPECT_TRUE(one->count());
    EXPECT_EQ(answers1, posit);

    args = {"--", "--one", "two", "three"};
    std::vector<std::string> answers2 = {"--one", "two", "three"};
    run();

    EXPECT_FALSE(one->count());
    EXPECT_EQ(answers2, posit);
}

TEST_F(TApp, MixedPositionals) {

    int positional_int;
    std::string positional_string;
    app.add_option("posit1,--posit1", positional_int, "");
    app.add_option("posit2,--posit2", positional_string, "");

    args = {"--posit2", "thing2", "7"};

    run();

    EXPECT_EQ(1u, app.count("posit2"));
    EXPECT_EQ(1u, app.count("--posit1"));
    EXPECT_EQ(7, positional_int);
    EXPECT_EQ("thing2", positional_string);
}

TEST_F(TApp, BigPositional) {
    std::vector<std::string> vec;
    app.add_option("pos", vec);

    args = {"one"};

    run();
    EXPECT_EQ(args, vec);

    args = {"one", "two"};
    run();

    EXPECT_EQ(args, vec);
}

TEST_F(TApp, Reset) {

    app.add_flag("--simple");
    double doub;
    app.add_option("-d,--double", doub);

    args = {"--simple", "--double", "1.2"};

    run();

    EXPECT_EQ(1u, app.count("--simple"));
    EXPECT_EQ(1u, app.count("-d"));
    EXPECT_DOUBLE_EQ(1.2, doub);

    app.clear();

    EXPECT_EQ(0u, app.count("--simple"));
    EXPECT_EQ(0u, app.count("-d"));

    run();

    EXPECT_EQ(1u, app.count("--simple"));
    EXPECT_EQ(1u, app.count("-d"));
    EXPECT_DOUBLE_EQ(1.2, doub);
}

TEST_F(TApp, RemoveOption) {
    app.add_flag("--one");
    auto opt = app.add_flag("--two");

    EXPECT_TRUE(app.remove_option(opt));
    EXPECT_FALSE(app.remove_option(opt));

    args = {"--two"};

    EXPECT_THROW(run(), CLI::ExtrasError);
}

TEST_F(TApp, RemoveNeedsLinks) {
    auto one = app.add_flag("--one");
    auto two = app.add_flag("--two");

    two->needs(one);
    one->needs(two);

    EXPECT_TRUE(app.remove_option(one));

    args = {"--two"};

    run();
}

TEST_F(TApp, RemoveExcludesLinks) {
    auto one = app.add_flag("--one");
    auto two = app.add_flag("--two");

    two->excludes(one);
    one->excludes(two);

    EXPECT_TRUE(app.remove_option(one));

    args = {"--two"};

    run(); // Mostly hoping it does not crash
}

TEST_F(TApp, FileNotExists) {
    std::string myfile{"TestNonFileNotUsed.txt"};
    ASSERT_NO_THROW(CLI::NonexistentPath(myfile));

    std::string filename;
    app.add_option("--file", filename)->check(CLI::NonexistentPath);
    args = {"--file", myfile};

    run();
    EXPECT_EQ(myfile, filename);

    bool ok = static_cast<bool>(std::ofstream(myfile.c_str()).put('a')); // create file
    EXPECT_TRUE(ok);
    EXPECT_THROW(run(), CLI::ValidationError);

    std::remove(myfile.c_str());
    EXPECT_FALSE(CLI::ExistingFile(myfile).empty());
}

TEST_F(TApp, FileExists) {
    std::string myfile{"TestNonFileNotUsed.txt"};
    EXPECT_FALSE(CLI::ExistingFile(myfile).empty());

    std::string filename = "Failed";
    app.add_option("--file", filename)->check(CLI::ExistingFile);
    args = {"--file", myfile};

    EXPECT_THROW(run(), CLI::ValidationError);

    bool ok = static_cast<bool>(std::ofstream(myfile.c_str()).put('a')); // create file
    EXPECT_TRUE(ok);
    run();
    EXPECT_EQ(myfile, filename);

    std::remove(myfile.c_str());
    EXPECT_FALSE(CLI::ExistingFile(myfile).empty());
}

TEST_F(TApp, InSet) {

    std::string choice;
    app.add_set("-q,--quick", choice, {"one", "two", "three"});

    args = {"--quick", "two"};

    run();
    EXPECT_EQ("two", choice);

    args = {"--quick", "four"};
    EXPECT_THROW(run(), CLI::ConversionError);
}

TEST_F(TApp, InSetWithDefault) {

    std::string choice = "one";
    app.add_set("-q,--quick", choice, {"one", "two", "three"}, "", true);

    run();
    EXPECT_EQ("one", choice);

    args = {"--quick", "two"};

    run();
    EXPECT_EQ("two", choice);

    args = {"--quick", "four"};
    EXPECT_THROW(run(), CLI::ConversionError);
}

TEST_F(TApp, InCaselessSetWithDefault) {

    std::string choice = "one";
    app.add_set_ignore_case("-q,--quick", choice, {"one", "two", "three"}, "", true);

    run();
    EXPECT_EQ("one", choice);

    args = {"--quick", "tWo"};

    run();
    EXPECT_EQ("two", choice);

    args = {"--quick", "four"};
    EXPECT_THROW(run(), CLI::ConversionError);
}

TEST_F(TApp, InIntSet) {

    int choice;
    app.add_set("-q,--quick", choice, {1, 2, 3});

    args = {"--quick", "2"};

    run();
    EXPECT_EQ(2, choice);

    args = {"--quick", "4"};
    EXPECT_THROW(run(), CLI::ConversionError);
}

TEST_F(TApp, InIntSetWindows) {

    int choice;
    app.add_set("-q,--quick", choice, {1, 2, 3});
    app.allow_windows_style_options();
    args = {"/q", "2"};

    run();
    EXPECT_EQ(2, choice);

    args = {"/q4"};
    EXPECT_THROW(run(), CLI::ExtrasError);
}

TEST_F(TApp, FailSet) {

    int choice;
    app.add_set("-q,--quick", choice, {1, 2, 3});

    args = {"--quick", "3", "--quick=2"};
    EXPECT_THROW(run(), CLI::ArgumentMismatch);

    args = {"--quick=hello"};
    EXPECT_THROW(run(), CLI::ConversionError);
}

TEST_F(TApp, FailMutableSet) {

    int choice;
    std::set<int> vals{1, 2, 3};
    app.add_mutable_set("-q,--quick", choice, vals);
    app.add_mutable_set("-s,--slow", choice, vals, "", true);

    args = {"--quick=hello"};
    EXPECT_THROW(run(), CLI::ConversionError);

    args = {"--slow=hello"};
    EXPECT_THROW(run(), CLI::ConversionError);
}

TEST_F(TApp, InSetIgnoreCase) {

    std::string choice;
    app.add_set_ignore_case("-q,--quick", choice, {"one", "Two", "THREE"});

    args = {"--quick", "One"};
    run();
    EXPECT_EQ("one", choice);

    args = {"--quick", "two"};
    run();
    EXPECT_EQ("Two", choice); // Keeps caps from set

    args = {"--quick", "ThrEE"};
    run();
    EXPECT_EQ("THREE", choice); // Keeps caps from set

    args = {"--quick", "four"};
    EXPECT_THROW(run(), CLI::ConversionError);

    args = {"--quick=one", "--quick=two"};
    EXPECT_THROW(run(), CLI::ArgumentMismatch);
}

TEST_F(TApp, InSetIgnoreCaseMutableValue) {

    std::set<std::string> options{"one", "Two", "THREE"};
    std::string choice;
    app.add_mutable_set_ignore_case("-q,--quick", choice, options);

    args = {"--quick", "One"};
    run();
    EXPECT_EQ("one", choice);

    args = {"--quick", "two"};
    run();
    EXPECT_EQ("Two", choice); // Keeps caps from set

    args = {"--quick", "ThrEE"};
    run();
    EXPECT_EQ("THREE", choice); // Keeps caps from set

    options.clear();
    args = {"--quick", "ThrEE"};
    EXPECT_THROW(run(), CLI::ConversionError);
}

TEST_F(TApp, InSetIgnoreCasePointer) {

    std::set<std::string> *options = new std::set<std::string>{"one", "Two", "THREE"};
    std::string choice;
    app.add_set_ignore_case("-q,--quick", choice, *options);

    args = {"--quick", "One"};
    run();
    EXPECT_EQ("one", choice);

    args = {"--quick", "two"};
    run();
    EXPECT_EQ("Two", choice); // Keeps caps from set

    args = {"--quick", "ThrEE"};
    run();
    EXPECT_EQ("THREE", choice); // Keeps caps from set

    delete options;
    args = {"--quick", "ThrEE"};
    run();
    EXPECT_EQ("THREE", choice); // this does not throw a segfault

    args = {"--quick", "four"};
    EXPECT_THROW(run(), CLI::ConversionError);

    args = {"--quick=one", "--quick=two"};
    EXPECT_THROW(run(), CLI::ArgumentMismatch);
}

TEST_F(TApp, InSetIgnoreUnderscore) {

    std::string choice;
    app.add_set_ignore_underscore("-q,--quick", choice, {"option_one", "option_two", "optionthree"});

    args = {"--quick", "option_one"};
    run();
    EXPECT_EQ("option_one", choice);

    args = {"--quick", "optiontwo"};
    run();
    EXPECT_EQ("option_two", choice); // Keeps underscore from set

    args = {"--quick", "_option_thr_ee"};
    run();
    EXPECT_EQ("optionthree", choice); // no underscore

    args = {"--quick", "Option4"};
    EXPECT_THROW(run(), CLI::ConversionError);

    args = {"--quick=option_one", "--quick=option_two"};
    EXPECT_THROW(run(), CLI::ArgumentMismatch);
}

TEST_F(TApp, InSetIgnoreCaseUnderscore) {

    std::string choice;
    app.add_set_ignore_case_underscore("-q,--quick", choice, {"Option_One", "option_two", "OptionThree"});

    args = {"--quick", "option_one"};
    run();
    EXPECT_EQ("Option_One", choice);

    args = {"--quick", "OptionTwo"};
    run();
    EXPECT_EQ("option_two", choice); // Keeps underscore and case from set

    args = {"--quick", "_OPTION_thr_ee"};
    run();
    EXPECT_EQ("OptionThree", choice); // no underscore

    args = {"--quick", "Option4"};
    EXPECT_THROW(run(), CLI::ConversionError);

    args = {"--quick=option_one", "--quick=option_two"};
    EXPECT_THROW(run(), CLI::ArgumentMismatch);
}

TEST_F(TApp, VectorFixedString) {
    std::vector<std::string> strvec;
    std::vector<std::string> answer{"mystring", "mystring2", "mystring3"};

    CLI::Option *opt = app.add_option("-s,--string", strvec)->expected(3);
    EXPECT_EQ(3, opt->get_expected());

    args = {"--string", "mystring", "mystring2", "mystring3"};
    run();
    EXPECT_EQ(3u, app.count("--string"));
    EXPECT_EQ(answer, strvec);
}

TEST_F(TApp, VectorDefaultedFixedString) {
    std::vector<std::string> strvec{"one"};
    std::vector<std::string> answer{"mystring", "mystring2", "mystring3"};

    CLI::Option *opt = app.add_option("-s,--string", strvec, "", true)->expected(3);
    EXPECT_EQ(3, opt->get_expected());

    args = {"--string", "mystring", "mystring2", "mystring3"};
    run();
    EXPECT_EQ(3u, app.count("--string"));
    EXPECT_EQ(answer, strvec);
}

TEST_F(TApp, VectorUnlimString) {
    std::vector<std::string> strvec;
    std::vector<std::string> answer{"mystring", "mystring2", "mystring3"};

    CLI::Option *opt = app.add_option("-s,--string", strvec);
    EXPECT_EQ(-1, opt->get_expected());

    args = {"--string", "mystring", "mystring2", "mystring3"};
    run();
    EXPECT_EQ(3u, app.count("--string"));
    EXPECT_EQ(answer, strvec);

    args = {"-s", "mystring", "mystring2", "mystring3"};
    run();
    EXPECT_EQ(3u, app.count("--string"));
    EXPECT_EQ(answer, strvec);
}

TEST_F(TApp, VectorFancyOpts) {
    std::vector<std::string> strvec;
    std::vector<std::string> answer{"mystring", "mystring2", "mystring3"};

    CLI::Option *opt = app.add_option("-s,--string", strvec)->required()->expected(3);
    EXPECT_EQ(3, opt->get_expected());

    args = {"--string", "mystring", "mystring2", "mystring3"};
    run();
    EXPECT_EQ(3u, app.count("--string"));
    EXPECT_EQ(answer, strvec);

    args = {"one", "two"};
    EXPECT_THROW(run(), CLI::RequiredError);

    EXPECT_THROW(run(), CLI::ParseError);
}

TEST_F(TApp, OriginalOrder) {
    std::vector<int> st1;
    CLI::Option *op1 = app.add_option("-a", st1);
    std::vector<int> st2;
    CLI::Option *op2 = app.add_option("-b", st2);

    args = {"-a", "1", "-b", "2", "-a3", "-a", "4"};

    run();

    EXPECT_EQ(st1, std::vector<int>({1, 3, 4}));
    EXPECT_EQ(st2, std::vector<int>({2}));

    EXPECT_EQ(app.parse_order(), std::vector<CLI::Option *>({op1, op2, op1, op1}));
}

TEST_F(TApp, NeedsFlags) {
    CLI::Option *opt = app.add_flag("-s,--string");
    app.add_flag("--both")->needs(opt);

    run();

    args = {"-s"};
    run();

    args = {"-s", "--both"};
    run();

    args = {"--both"};
    EXPECT_THROW(run(), CLI::RequiresError);
}

TEST_F(TApp, ExcludesFlags) {
    CLI::Option *opt = app.add_flag("-s,--string");
    app.add_flag("--nostr")->excludes(opt);

    run();

    args = {"-s"};
    run();

    args = {"--nostr"};
    run();

    args = {"--nostr", "-s"};
    EXPECT_THROW(run(), CLI::ExcludesError);

    args = {"--string", "--nostr"};
    EXPECT_THROW(run(), CLI::ExcludesError);
}

TEST_F(TApp, ExcludesMixedFlags) {
    CLI::Option *opt1 = app.add_flag("--opt1");
    app.add_flag("--opt2");
    CLI::Option *opt3 = app.add_flag("--opt3");
    app.add_flag("--no")->excludes(opt1, "--opt2", opt3);

    run();

    args = {"--no"};
    run();

    args = {"--opt2"};
    run();

    args = {"--no", "--opt1"};
    EXPECT_THROW(run(), CLI::ExcludesError);

    args = {"--no", "--opt2"};
    EXPECT_THROW(run(), CLI::ExcludesError);
}

TEST_F(TApp, NeedsMultiFlags) {
    CLI::Option *opt1 = app.add_flag("--opt1");
    CLI::Option *opt2 = app.add_flag("--opt2");
    CLI::Option *opt3 = app.add_flag("--opt3");
    app.add_flag("--optall")->needs(opt1, opt2, opt3);

    run();

    args = {"--opt1"};
    run();

    args = {"--opt2"};
    run();

    args = {"--optall"};
    EXPECT_THROW(run(), CLI::RequiresError);

    args = {"--optall", "--opt1"};
    EXPECT_THROW(run(), CLI::RequiresError);

    args = {"--optall", "--opt2", "--opt1"};
    EXPECT_THROW(run(), CLI::RequiresError);

    args = {"--optall", "--opt1", "--opt2", "--opt3"};
    run();
}

TEST_F(TApp, NeedsMixedFlags) {
    CLI::Option *opt1 = app.add_flag("--opt1");
    app.add_flag("--opt2");
    app.add_flag("--opt3");
    app.add_flag("--optall")->needs(opt1, "--opt2", "--opt3");

    run();

    args = {"--opt1"};
    run();

    args = {"--opt2"};
    run();

    args = {"--optall"};
    EXPECT_THROW(run(), CLI::RequiresError);

    args = {"--optall", "--opt1"};
    EXPECT_THROW(run(), CLI::RequiresError);

    args = {"--optall", "--opt2", "--opt1"};
    EXPECT_THROW(run(), CLI::RequiresError);

    args = {"--optall", "--opt1", "--opt2", "--opt3"};
    run();
}

TEST_F(TApp, NeedsChainedFlags) {
    CLI::Option *opt1 = app.add_flag("--opt1");
    CLI::Option *opt2 = app.add_flag("--opt2")->needs(opt1);
    app.add_flag("--opt3")->needs(opt2);

    run();

    args = {"--opt1"};
    run();

    args = {"--opt2"};
    EXPECT_THROW(run(), CLI::RequiresError);

    args = {"--opt3"};
    EXPECT_THROW(run(), CLI::RequiresError);

    args = {"--opt3", "--opt2"};
    EXPECT_THROW(run(), CLI::RequiresError);

    args = {"--opt3", "--opt1"};
    EXPECT_THROW(run(), CLI::RequiresError);

    args = {"--opt2", "--opt1"};
    run();

    args = {"--opt1", "--opt2", "--opt3"};
    run();
}

TEST_F(TApp, Env) {

    put_env("CLI11_TEST_ENV_TMP", "2");

    int val = 1;
    CLI::Option *vopt = app.add_option("--tmp", val)->envname("CLI11_TEST_ENV_TMP");

    run();

    EXPECT_EQ(2, val);
    EXPECT_EQ(1u, vopt->count());

    vopt->required();
    run();

    unset_env("CLI11_TEST_ENV_TMP");
    EXPECT_THROW(run(), CLI::RequiredError);
}

TEST_F(TApp, RangeInt) {
    int x = 0;
    app.add_option("--one", x)->check(CLI::Range(3, 6));

    args = {"--one=1"};
    EXPECT_THROW(run(), CLI::ValidationError);

    args = {"--one=7"};
    EXPECT_THROW(run(), CLI::ValidationError);

    args = {"--one=3"};
    run();

    args = {"--one=5"};
    run();

    args = {"--one=6"};
    run();
}

TEST_F(TApp, RangeDouble) {

    double x = 0;
    /// Note that this must be a double in Range, too
    app.add_option("--one", x)->check(CLI::Range(3.0, 6.0));

    args = {"--one=1"};
    EXPECT_THROW(run(), CLI::ValidationError);

    args = {"--one=7"};
    EXPECT_THROW(run(), CLI::ValidationError);

    args = {"--one=3"};
    run();

    args = {"--one=5"};
    run();

    args = {"--one=6"};
    run();
}

// Check to make sure programmatic access to left over is available
TEST_F(TApp, AllowExtras) {

    app.allow_extras();

    bool val = true;
    app.add_flag("-f", val);
    EXPECT_FALSE(val);

    args = {"-x", "-f"};

    ASSERT_NO_THROW(run());
    EXPECT_TRUE(val);
    EXPECT_EQ(app.remaining(), std::vector<std::string>({"-x"}));
}

TEST_F(TApp, AllowExtrasOrder) {

    app.allow_extras();

    args = {"-x", "-f"};
    ASSERT_NO_THROW(run());
    EXPECT_EQ(app.remaining(), std::vector<std::string>({"-x", "-f"}));

    std::vector<std::string> left_over = app.remaining();
    app.parse(left_over);
    EXPECT_EQ(app.remaining(), left_over);
}

// Test horrible error
TEST_F(TApp, CheckShortFail) {
    args = {"--two"};

    EXPECT_THROW(CLI::detail::AppFriend::parse_arg(&app, args, CLI::detail::Classifier::SHORT), CLI::HorribleError);
}

// Test horrible error
TEST_F(TApp, CheckLongFail) {
    args = {"-t"};

    EXPECT_THROW(CLI::detail::AppFriend::parse_arg(&app, args, CLI::detail::Classifier::LONG), CLI::HorribleError);
}

// Test horrible error
TEST_F(TApp, CheckWindowsFail) {
    args = {"-t"};

    EXPECT_THROW(CLI::detail::AppFriend::parse_arg(&app, args, CLI::detail::Classifier::WINDOWS), CLI::HorribleError);
}

// Test horrible error
TEST_F(TApp, CheckOtherFail) {
    args = {"-t"};

    EXPECT_THROW(CLI::detail::AppFriend::parse_arg(&app, args, CLI::detail::Classifier::NONE), CLI::HorribleError);
}

// Test horrible error
TEST_F(TApp, CheckSubcomFail) {
    args = {"subcom"};

    EXPECT_THROW(CLI::detail::AppFriend::parse_subcommand(&app, args), CLI::HorribleError);
}

TEST_F(TApp, OptionWithDefaults) {
    int someint = 2;
    app.add_option("-a", someint, "", true);

    args = {"-a1", "-a2"};

    EXPECT_THROW(run(), CLI::ArgumentMismatch);
}

TEST_F(TApp, SetWithDefaults) {
    int someint = 2;
    app.add_set("-a", someint, {1, 2, 3, 4}, "", true);

    args = {"-a1", "-a2"};

    EXPECT_THROW(run(), CLI::ArgumentMismatch);
}

TEST_F(TApp, SetWithDefaultsConversion) {
    int someint = 2;
    app.add_set("-a", someint, {1, 2, 3, 4}, "", true);

    args = {"-a", "hi"};

    EXPECT_THROW(run(), CLI::ConversionError);
}

TEST_F(TApp, SetWithDefaultsIC) {
    std::string someint = "ho";
    app.add_set_ignore_case("-a", someint, {"Hi", "Ho"}, "", true);

    args = {"-aHi", "-aHo"};

    EXPECT_THROW(run(), CLI::ArgumentMismatch);
}

// Added to test ->transform
TEST_F(TApp, OrderedModifingTransforms) {
    std::vector<std::string> val;
    auto m = app.add_option("-m", val);
    m->transform([](std::string x) { return x + "1"; });
    m->transform([](std::string x) { return x + "2"; });

    args = {"-mone", "-mtwo"};

    run();

    EXPECT_EQ(val, std::vector<std::string>({"one12", "two12"}));
}

TEST_F(TApp, ThrowingTransform) {
    std::string val;
    auto m = app.add_option("-m,--mess", val);
    m->transform([](std::string) -> std::string { throw CLI::ValidationError("My Message"); });

    ASSERT_NO_THROW(run());

    args = {"-mone"};

    ASSERT_THROW(run(), CLI::ValidationError);

    try {
        run();
    } catch(const CLI::ValidationError &e) {
        EXPECT_EQ(e.what(), std::string("--mess: My Message"));
    }
}

// This was added to make running a simple function on each item easier
TEST_F(TApp, EachItem) {

    std::vector<std::string> results;
    std::vector<std::string> dummy;

    auto opt = app.add_option("--vec", dummy);

    opt->each([&results](std::string item) { results.push_back(item); });

    args = {"--vec", "one", "two", "three"};

    run();

    EXPECT_EQ(results, dummy);
}

// #87
TEST_F(TApp, CustomDoubleOption) {

    std::pair<int, double> custom_opt;

    auto opt = app.add_option("posit", [&custom_opt](CLI::results_t vals) {
        custom_opt = {stol(vals.at(0)), stod(vals.at(1))};
        return true;
    });
    opt->type_name("INT FLOAT")->type_size(2);

    args = {"12", "1.5"};

    run();
    EXPECT_EQ(custom_opt.first, 12);
    EXPECT_DOUBLE_EQ(custom_opt.second, 1.5);
}

// #113
TEST_F(TApp, AddRemoveSetItems) {
    std::set<std::string> items{"TYPE1", "TYPE2", "TYPE3", "TYPE4", "TYPE5"};

    std::string type1, type2;
    app.add_mutable_set("--type1", type1, items);
    app.add_mutable_set("--type2", type2, items, "", true);

    args = {"--type1", "TYPE1", "--type2", "TYPE2"};

    run();
    EXPECT_EQ(type1, "TYPE1");
    EXPECT_EQ(type2, "TYPE2");

    items.insert("TYPE6");
    items.insert("TYPE7");

    items.erase("TYPE1");
    items.erase("TYPE2");

    args = {"--type1", "TYPE6", "--type2", "TYPE7"};
    run();
    EXPECT_EQ(type1, "TYPE6");
    EXPECT_EQ(type2, "TYPE7");

    args = {"--type1", "TYPE1"};
    EXPECT_THROW(run(), CLI::ConversionError);

    args = {"--type2", "TYPE2"};
    EXPECT_THROW(run(), CLI::ConversionError);
}

TEST_F(TApp, AddRemoveSetItemsNoCase) {
    std::set<std::string> items{"TYPE1", "TYPE2", "TYPE3", "TYPE4", "TYPE5"};

    std::string type1, type2;
    app.add_mutable_set_ignore_case("--type1", type1, items);
    app.add_mutable_set_ignore_case("--type2", type2, items, "", true);

    args = {"--type1", "TYPe1", "--type2", "TyPE2"};

    run();
    EXPECT_EQ(type1, "TYPE1");
    EXPECT_EQ(type2, "TYPE2");

    items.insert("TYPE6");
    items.insert("TYPE7");

    items.erase("TYPE1");
    items.erase("TYPE2");

    args = {"--type1", "TyPE6", "--type2", "tYPE7"};
    run();
    EXPECT_EQ(type1, "TYPE6");
    EXPECT_EQ(type2, "TYPE7");

    args = {"--type1", "TYPe1"};
    EXPECT_THROW(run(), CLI::ConversionError);

    args = {"--type2", "TYpE2"};
    EXPECT_THROW(run(), CLI::ConversionError);
}

// #128
TEST_F(TApp, RepeatingMultiArgumentOptions) {
    std::vector<std::string> entries;
    app.add_option("--entry", entries, "set a key and value")->type_name("KEY VALUE")->type_size(-2);

    args = {"--entry", "key1", "value1", "--entry", "key2", "value2"};
    ASSERT_NO_THROW(run());
    EXPECT_EQ(entries, std::vector<std::string>({"key1", "value1", "key2", "value2"}));

    args.pop_back();
    ASSERT_THROW(run(), CLI::ArgumentMismatch);
}

// #122
TEST_F(TApp, EmptyOptionEach) {
    std::string q;
    app.add_option("--each", {})->each([&q](std::string s) { q = s; });

    args = {"--each", "that"};
    run();

    EXPECT_EQ(q, "that");
}

// #122
TEST_F(TApp, EmptyOptionFail) {
    std::string q;
    app.add_option("--each", {});

    args = {"--each", "that"};
    run();
}

TEST_F(TApp, BeforeRequirements) {
    app.add_flag_function("-a", [](size_t) { throw CLI::Success(); });
    app.add_flag_function("-b", [](size_t) { throw CLI::CallForHelp(); });

    args = {"extra"};
    EXPECT_THROW(run(), CLI::ExtrasError);

    args = {"-a", "extra"};
    EXPECT_THROW(run(), CLI::Success);

    args = {"-b", "extra"};
    EXPECT_THROW(run(), CLI::CallForHelp);

    // These run in definition order.
    args = {"-a", "-b", "extra"};
    EXPECT_THROW(run(), CLI::Success);

    // Currently, the original order is not preserved when calling callbacks
    // args = {"-b", "-a", "extra"};
    // EXPECT_THROW(run(), CLI::CallForHelp);
}

// #209
TEST_F(TApp, CustomUserSepParse) {

    std::vector<int> vals = {1, 2, 3};
    args = {"--idx", "1,2,3"};
    auto opt = app.add_option("--idx", vals, "", ',');
    run();
    EXPECT_EQ(vals, std::vector<int>({1, 2, 3}));

    app.remove_option(opt);

    app.add_option("--idx", vals, "", true, ',');
    run();
    EXPECT_EQ(vals, std::vector<int>({1, 2, 3}));
}

// #209
TEST_F(TApp, DefaultUserSepParse) {

    std::vector<std::string> vals;
    args = {"--idx", "1 2 3", "4 5 6"};
    auto opt = app.add_option("--idx", vals, "");
    run();
    EXPECT_EQ(vals, std::vector<std::string>({"1 2 3", "4 5 6"}));
    app.remove_option(opt);
    app.add_option("--idx", vals, "", true);
    run();
    EXPECT_EQ(vals, std::vector<std::string>({"1 2 3", "4 5 6"}));
}

// #209
TEST_F(TApp, BadUserSepParse) {

    std::vector<int> vals;
    app.add_option("--idx", vals, "");

    args = {"--idx", "1,2,3"};

    EXPECT_THROW(run(), CLI::ConversionError);
}

// #209
TEST_F(TApp, CustomUserSepParse2) {

    std::vector<int> vals = {1, 2, 3};
    args = {"--idx", "1,2,"};
    auto opt = app.add_option("--idx", vals, "", ',');
    run();
    EXPECT_EQ(vals, std::vector<int>({1, 2}));

    app.remove_option(opt);

    app.add_option("--idx", vals, "", true, ',');
    run();
    EXPECT_EQ(vals, std::vector<int>({1, 2}));
}

// #209
TEST_F(TApp, CustomUserSepParse3) {

    std::vector<int> vals = {1, 2, 3};
    args = {"--idx",
            "1",
            ","
            "2"};
    auto opt = app.add_option("--idx", vals, "", ',');
    run();
    EXPECT_EQ(vals, std::vector<int>({1, 2}));
    app.remove_option(opt);

    app.add_option("--idx", vals, "", false, ',');
    run();
    EXPECT_EQ(vals, std::vector<int>({1, 2}));
}

// #209
TEST_F(TApp, CustomUserSepParse4) {

    std::vector<int> vals;
    args = {"--idx", "1,    2"};
    auto opt = app.add_option("--idx", vals, "", ',');
    run();
    EXPECT_EQ(vals, std::vector<int>({1, 2}));

    app.remove_option(opt);

    app.add_option("--idx", vals, "", true, ',');
    run();
    EXPECT_EQ(vals, std::vector<int>({1, 2}));
}

// #218
TEST_F(TApp, CustomUserSepParse5) {

    std::vector<std::string> bar;
    args = {"this", "is", "a", "test"};
    auto opt = app.add_option("bar", bar, "bar");
    run();
    EXPECT_EQ(bar, std::vector<std::string>({"this", "is", "a", "test"}));

    app.remove_option(opt);
    args = {"this", "is", "a", "test"};
    app.add_option("bar", bar, "bar", true);
    run();
    EXPECT_EQ(bar, std::vector<std::string>({"this", "is", "a", "test"}));
}
