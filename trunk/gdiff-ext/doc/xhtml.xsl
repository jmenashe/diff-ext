<?xml version="1.0" encoding="utf-8"?> 
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:import href="c:/devel/docbook/docbook-xsl-1.72.0/xhtml/docbook.xsl"/>
<xsl:output method="xml" 
    encoding="utf-8" 
    indent="yes"/>
<!-- xsl:output method="xml" 
    encoding="utf-8" 
    indent="yes" 
    doctype-public="-//W3C//DTD XHTML 1.1//EN" 
    doctype-system="http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd"/-->
<xsl:param name="chunker.output.encoding" select="'utf-8'"/>
<xsl:param name="section.autolabel" select="1"/>
<xsl:param name="html.stylesheet" select="'style.css'"/>
<xsl:param name="footnote.number.symbols" select="'*&#x2020;&#x2021;&#x25CA;&#x2720;'"></xsl:param>
<!--xsl:param name="table.footnote.number.symbols" select="'*&#x2020;&#x2021;&#x25CA;&#x2720;'"></xsl:param-->
<!--xsl:param name="generate.toc" select="'article nop'"/-->
</xsl:stylesheet>
