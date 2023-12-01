/*
 * Summary: interface for an HTML 4.0 non-verifying parser
 * Description: this module implements an HTML 4.0 non-verifying parser
 *              with API compatible with the XML parser ones. It should
 *              be able to parse "real world" HTML, even if severely
 *              broken from a specification point of view.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __HTML_PARSER_H__
#define __HTML_PARSER_H__
#include <libxml/xmlversion.h>
#include <libxml/parser.h>

#ifdef LIBXML_HTML_ENABLED

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Most of the back-end structures from XML and HTML are shared.
 */
typedef xmlParserCtxt htmlParserCtxt;
typedef xmlParserCtxtPtr htmlParserCtxtPtr;
typedef xmlParserNodeInfo htmlParserNodeInfo;
typedef xmlSAXHandler htmlSAXHandler;
typedef xmlSAXHandlerPtr htmlSAXHandlerPtr;
typedef xmlParserInput htmlParserInput;
typedef xmlParserInputPtr htmlParserInputPtr;
typedef xmlDocPtr htmlDocPtr;
typedef xmlNodePtr htmlNodePtr;

/*
 * Internal description of an HTML element, representing HTML 4.01
 * and XHTML 1.0 (which share the same structure).
 */
typedef struct _htmlElemDesc htmlElemDesc;
typedef htmlElemDesc *htmlElemDescPtr;
struct _htmlElemDesc {
    const char *name;	/* The tag name */
    char startTag;      /* Whether the start tag can be implied */
    char endTag;        /* Whether the end tag can be implied */
    char saveEndTag;    /* Whether the end tag should be saved */
    char empty;         /* Is this an empty element ? */
    char depr;          /* Is this a deprecated element ? */
    char dtd;           /* 1: only in Loose DTD, 2: only Frameset one */
    char isinline;      /* is this a block 0 or inline 1 element */
    const char *desc;   /* the description */

/* NRK Jan.2003
 * New fields encapsulating HTML structure
 *
 * Bugs:
 *	This is a very limited representation.  It fails to tell us when
 *	an element *requires* subelements (we only have whether they're
 *	allowed or not), and it doesn't tell us where CDATA and PCDATA
 *	are allowed.  Some element relationships are not fully represented:
 *	these are flagged with the word MODIFIER
 */
    const char** subelts;		/* allowed sub-elements of this element */
    const char* defaultsubelt;	/* subelement for suggested auto-repair
					   if necessary or NULL */
    const char** attrs_opt;		/* Optional Attributes */
    const char** attrs_depr;		/* Additional deprecated attributes */
    const char** attrs_req;		/* Required attributes */
};

/*
 * Internal description of an HTML entity.
 */
typedef struct _htmlEntityDesc htmlEntityDesc;
typedef htmlEntityDesc *htmlEntityDescPtr;
struct _htmlEntityDesc {
    unsigned int value;	/* the UNICODE value for the character */
    const char *name;	/* The entity name */
    const char *desc;   /* the description */
};

/** DOC_DISABLE */
#ifdef LIBXML_SAX1_ENABLED
  #define XML_GLOBALS_HTML \
    XML_OP(htmlDefaultSAXHandler, xmlSAXHandlerV1, XML_DEPRECATED)
#else
  #define XML_GLOBALS_HTML
#endif

#define XML_OP XML_DECLARE_GLOBAL
XML_GLOBALS_HTML
#undef XML_OP

#if defined(LIBXML_THREAD_ENABLED) && !defined(XML_GLOBALS_NO_REDEFINITION)
  #define htmlDefaultSAXHandler XML_GLOBAL_MACRO(htmlDefaultSAXHandler)
#endif
/** DOC_ENABLE */

/*
 * There is only few public functions.
 */
XML_DEPRECATED
XMLPUBFUN void
			htmlInitAutoClose	(void);
XMLPUBFUN const htmlElemDesc *
			htmlTagLookup	(const xmlChar *tag);
XMLPUBFUN const htmlEntityDesc *
			htmlEntityLookup(const xmlChar *name);
XMLPUBFUN const htmlEntityDesc *
			htmlEntityValueLookup(unsigned int value);

XMLPUBFUN int
			htmlIsAutoClosed(htmlDocPtr doc,
					 htmlNodePtr elem);
XMLPUBFUN int
			htmlAutoCloseTag(htmlDocPtr doc,
					 const xmlChar *name,
					 htmlNodePtr elem);
XML_DEPRECATED
XMLPUBFUN const htmlEntityDesc *
			htmlParseEntityRef(htmlParserCtxtPtr ctxt,
					 const xmlChar **str);
XML_DEPRECATED
XMLPUBFUN int
			htmlParseCharRef(htmlParserCtxtPtr ctxt);
XML_DEPRECATED
XMLPUBFUN void
			htmlParseElement(htmlParserCtxtPtr ctxt);

XMLPUBFUN htmlParserCtxtPtr
			htmlNewParserCtxt(void);
XMLPUBFUN htmlParserCtxtPtr
			htmlNewSAXParserCtxt(const htmlSAXHandler *sax,
					     void *userData);

XMLPUBFUN htmlParserCtxtPtr
			htmlCreateMemoryParserCtxt(const char *buffer,
						   int size);

XMLPUBFUN int
			htmlParseDocument(htmlParserCtxtPtr ctxt);
XML_DEPRECATED
XMLPUBFUN htmlDocPtr
			htmlSAXParseDoc	(const xmlChar *cur,
					 const char *encoding,
					 htmlSAXHandlerPtr sax,
					 void *userData);
XMLPUBFUN htmlDocPtr
			htmlParseDoc	(const xmlChar *cur,
					 const char *encoding);
XMLPUBFUN htmlParserCtxtPtr
			htmlCreateFileParserCtxt(const char *filename,
	                                         const char *encoding);
XML_DEPRECATED
XMLPUBFUN htmlDocPtr
			htmlSAXParseFile(const char *filename,
					 const char *encoding,
					 htmlSAXHandlerPtr sax,
					 void *userData);
XMLPUBFUN htmlDocPtr
			htmlParseFile	(const char *filename,
					 const char *encoding);
XMLPUBFUN int
			UTF8ToHtml	(unsigned char *out,
					 int *outlen,
					 const unsigned char *in,
					 int *inlen);
XMLPUBFUN int
			htmlEncodeEntities(unsigned char *out,
					 int *outlen,
					 const unsigned char *in,
					 int *inlen, int quoteChar);
XMLPUBFUN int
			htmlIsScriptAttribute(const xmlChar *name);
XMLPUBFUN int
			htmlHandleOmittedElem(int val);

#ifdef LIBXML_PUSH_ENABLED
/**
 * Interfaces for the Push mode.
 */
XMLPUBFUN htmlParserCtxtPtr
			htmlCreatePushParserCtxt(htmlSAXHandlerPtr sax,
						 void *user_data,
						 const char *chunk,
						 int size,
						 const char *filename,
						 xmlCharEncoding enc);
XMLPUBFUN int
			htmlParseChunk		(htmlParserCtxtPtr ctxt,
						 const char *chunk,
						 int size,
						 int terminate);
#endif /* LIBXML_PUSH_ENABLED */

XMLPUBFUN void
			htmlFreeParserCtxt	(htmlParserCtxtPtr ctxt);

/*
 * New set of simpler/more flexible APIs
 */
/**
 * xmlParserOption:
 *
 * This is the set of XML parser options that can be passed down
 * to the xmlReadDoc() and similar calls.
 */
typedef enum {
    HTML_PARSE_RECOVER  = 1<<0, /* Relaxed parsing */
    HTML_PARSE_NODEFDTD = 1<<2, /* do not default a doctype if not found */
    HTML_PARSE_NOERROR	= 1<<5,	/* suppress error reports */
    HTML_PARSE_NOWARNING= 1<<6,	/* suppress warning reports */
    HTML_PARSE_PEDANTIC	= 1<<7,	/* pedantic error reporting */
    HTML_PARSE_NOBLANKS	= 1<<8,	/* remove blank nodes */
    HTML_PARSE_NONET	= 1<<11,/* Forbid network access */
    HTML_PARSE_NOIMPLIED= 1<<13,/* Do not add implied html/body... elements */
    HTML_PARSE_COMPACT  = 1<<16,/* compact small text nodes */
    HTML_PARSE_IGNORE_ENC=1<<21 /* ignore internal document encoding hint */
} htmlParserOption;

XMLPUBFUN void
		htmlCtxtReset		(htmlParserCtxtPtr ctxt);
XMLPUBFUN int
		htmlCtxtUseOptions	(htmlParserCtxtPtr ctxt,
					 int options);
XMLPUBFUN htmlDocPtr
		htmlReadDoc		(const xmlChar *cur,
					 const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN htmlDocPtr
		htmlReadFile		(const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN htmlDocPtr
		htmlReadMemory		(const char *buffer,
					 int size,
					 const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN htmlDocPtr
		htmlReadFd		(int fd,
					 const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN htmlDocPtr
		htmlReadIO		(xmlInputReadCallback ioread,
					 xmlInputCloseCallback ioclose,
					 void *ioctx,
					 const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN htmlDocPtr
		htmlCtxtReadDoc		(xmlParserCtxtPtr ctxt,
					 const xmlChar *cur,
					 const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN htmlDocPtr
		htmlCtxtReadFile		(xmlParserCtxtPtr ctxt,
					 const char *filename,
					 const char *encoding,
					 int options);
XMLPUBFUN htmlDocPtr
		htmlCtxtReadMemory		(xmlParserCtxtPtr ctxt,
					 const char *buffer,
					 int size,
					 const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN htmlDocPtr
		htmlCtxtReadFd		(xmlParserCtxtPtr ctxt,
					 int fd,
					 const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN htmlDocPtr
		htmlCtxtReadIO		(xmlParserCtxtPtr ctxt,
					 xmlInputReadCallback ioread,
					 xmlInputCloseCallback ioclose,
					 void *ioctx,
					 const char *URL,
					 const char *encoding,
					 int options);

/* NRK/Jan2003: further knowledge of HTML structure
 */
typedef enum {
  HTML_NA = 0 ,		/* something we don't check at all */
  HTML_INVALID = 0x1 ,
  HTML_DEPRECATED = 0x2 ,
  HTML_VALID = 0x4 ,
  HTML_REQUIRED = 0xc /* VALID bit set so ( & HTML_VALID ) is TRUE */
} htmlStatus ;

/* Using htmlElemDesc rather than name here, to emphasise the fact
   that otherwise there's a lookup overhead
*/
XMLPUBFUN htmlStatus htmlAttrAllowed(const htmlElemDesc*, const xmlChar*, int) ;
XMLPUBFUN int htmlElementAllowedHere(const htmlElemDesc*, const xmlChar*) ;
XMLPUBFUN htmlStatus htmlElementStatusHere(const htmlElemDesc*, const htmlElemDesc*) ;
XMLPUBFUN htmlStatus htmlNodeStatus(htmlNodePtr, int) ;
/**
 * htmlDefaultSubelement:
 * @elt: HTML element
 *
 * Returns the default subelement for this element
 */
#define htmlDefaultSubelement(elt) elt->defaultsubelt
/**
 * htmlElementAllowedHereDesc:
 * @parent: HTML parent element
 * @elt: HTML element
 *
 * Checks whether an HTML element description may be a
 * direct child of the specified element.
 *
 * Returns 1 if allowed; 0 otherwise.
 */
#define htmlElementAllowedHereDesc(parent,elt) \
	htmlElementAllowedHere((parent), (elt)->name)
/**
 * htmlRequiredAttrs:
 * @elt: HTML element
 *
 * Returns the attributes required for the specified element.
 */
#define htmlRequiredAttrs(elt) (elt)->attrs_req


#ifdef __cplusplus
}
#endif

#else /* LIBXML_HTML_ENABLED */

/** DOC_DISABLE */
#define XML_GLOBALS_HTML
/** DOC_ENABLE */

#endif /* LIBXML_HTML_ENABLED */
#endif /* __HTML_PARSER_H__ */
