//
// Created by ferris on 7/6/22.
//

#ifndef DICTGENERATOR_TOOLS_H
#define DICTGENERATOR_TOOLS_H

#include <vector>
#include "map"
#include "string"

using namespace std;

vector<string> SplitString(const string & sourceString);

vector<string> SplitString(const string & sourceString, const char & delimiter);

vector<string> SplitProps(const string & sourceString);

[[nodiscard]]bool StringNotBlank (string &);
string RemoveNewline(string s);
string Trim(string s);

[[nodiscard]]bool StringInVector(vector<string>, string&);

string FileToString(const string& fileName);
string FileToString(const string& fileName, const string& startMarker, const string& endMarker);

string NewAttr(string name, string text);

// decoder
static map<string, string> charMap {
        {"&#160;", " "},
        {"&#169;", "©"},
        {"&#187;", "»"},
        {"&#257;", "ā"},
        {"&#263;", "ć"},
        {"&#268;", "Č"},
        {"&#269;", "č"},
        {"&#275;", "ē"},
        {"&#299;", "ī"},
        {"&#301;", "ĭ"},
        {"&#333;", "ō"},
        {"&#596;", "ɔ"},
        {"&#600;", "ɘ"},
        {"&#603;", "ɛ"},
        {"&#643;", "ʃ"},
        {"&#654;", "ʎ"},
        {"&#658;", "ʒ"},
        {"&#703;", "ʿ"},
        {"&#728;", "˘"},
        {"&#7703;", "ḗ"},
        {"&#7779;", "ṣ"},
        {"&#7827;", "ẓ"},
        {"&#8212;", "—"},
        {"&#8216;", "‘"},
        {"&#8217;", "'"},
        {"&#39;", "'"},
        {"&#8220;", "“"},
        {"&#8221;", "”"},
        {"&#8230;", "…"},
        {"&aacute;", "á"},
        {"&acirc;", "â"},
        {"&aelig;", "æ"},
        {"&agrave;", "à"},
        {"&Agrave;", "À"},
        {"&alpha;", "α"},
        {"&Alpha;", "Α"},
        {"&amp;", "&"},
        {"&Aring;", "Å"},
        {"&atilde;", "ã"},
        {"&auml;", "ä"},
        {"&beta;", "β"},
        {"&Beta;", "Β"},
        {"&ccedil;", "ç"},
        {"&chi;", "χ"},
        {"&Chi;", "Χ"},
        {"&deg;", "°"},
        {"&delta;", "δ"},
        {"&Delta;", "Δ"},
        {"&eacute;", "é"},
        {"&ecirc;", "ê"},
        {"&egrave;", "è"},
        {"&epsilon;", "ε"},
        {"&Epsilon;", "Ε"},
        {"&eta;", "η"},
        {"&Eta;", "Η"},
        {"&eth;", "ð"},
        {"&euml;", "ë"},
        {"&gamma;", "γ"},
        {"&Gamma;", "Γ"},
        {"&gt;", ">"},
        {"&hellip;", "…"},
        {"&iacute;", "í"},
        {"&icirc;", "î"},
        {"&igrave;", "ì"},
        {"&iota;", "ι"},
        {"&Iota;", "Ι"},
        {"&iuml;", "ï"},
        {"&kappa;", "κ"},
        {"&Kappa;", "Κ"},
        {"&lambda;", "λ"},
        {"&Lambda;", "Λ"},
        {"&laquo;", "«"},
        {"&lt;", "<"},
        {"&macr;", "¯"},
        {"&mu;", "μ"},
        {"&Mu;", "Μ"},
        {"&nbsp;", " "},
        {"&ndash;", "–"},
        {"&ntilde;", "ñ"},
        {"&nu;", "ν"},
        {"&Nu;", "Ν"},
        {"&oacute;", "ó"},
        {"&ocirc;", "ô"},
        {"&oelig;", "œ"},
        {"&ograve;", "ò"},
        {"&omega;", "ω"},
        {"&Omega;", "Ω"},
        {"&omicron;", "ο"},
        {"&Omicron;", "Ο"},
        {"&ordf;", "ª"},
        {"&oslash;", "ø"},
        {"&otilde;", "õ"},
        {"&ouml;", "ö"},
        {"&phi;", "φ"},
        {"&Phi;", "Φ"},
        {"&pi;", "π"},
        {"&Pi;", "Π"},
        {"&psi;", "ψ"},
        {"&quot;", "\""},
        {"&raquo;", "»"},
        {"&rarr;", "→"},
        {"&rdquo;", "”"},
        {"&reg;", "®"},
        {"&rho;", "ρ"},
        {"&Rho;", "Ρ"},
        {"&rsquo;", "’"},
        {"&scaron;", "š"},
        {"&Sigma;", "Σ"},
        {"&sigma;", "σ"},
        {"&sigmaf;", "ς"},
        {"&szlig;", "ß"},
        {"&tau;", "τ"},
        {"&Tau;", "Τ"},
        {"&theta;", "θ"},
        {"&Theta;", "Θ"},
        {"&times;", "×"},
        {"&uacute;", "ú"},
        {"&ucirc;", "û"},
        {"&ugrave;", "ù"},
        {"&upsilon;", "υ"},
        {"&Upsilon;", "Υ"},
        {"&uuml;", "ü"},
        {"&xi;", "ξ"},
        {"&Xi;", "Ξ"},
        {"&zeta;", "ζ"},
        {"&Zeta;", "Ζ"}
};

string Decode(string & str);
int GetNumbering(string & str);

string RemoveNumber(string & str);

string Italicize(string &str);

#endif //DICTGENERATOR_TOOLS_H