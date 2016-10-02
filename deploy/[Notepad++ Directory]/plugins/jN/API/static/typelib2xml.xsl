<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html" encoding="utf-8" doctype-public="-//W3C//DTD XHTML 1.0 Strict//EN"  doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"/>
 
<xsl:template match="/">
 <html><head>
 <title><xsl:value-of select="TypeLibrary/@name" /> - API</title>
 <link rel="stylesheet" type="text/css" href="static/typelib2xml.css"/>
 <meta http-equiv="content-type" content="text/html; charset=utf-8" />

 </head><body>
	<xsl:apply-templates select="TypeLibrary"/>
	
 </body>
 </html>
</xsl:template>

<xsl:template match="TypeLibrary">

	<h1><xsl:value-of select="@name" /></h1>
	<div>
		<xsl:apply-templates select="documentation" />
	</div>
	<xsl:apply-templates select="type" />
	
</xsl:template>

<xsl:template match="type">
	<div class="type" id="{generate-id()}">


	<h2><xsl:value-of select="@name" /></h2>
	<div class="documentation">
		<xsl:apply-templates select="documentation" />
	</div>
	
	<div class="implements">
		<b>Implements: </b><xsl:apply-templates select="impl"/>
	</div>

	<table>
		<tr>
			<th></th>
			<th>Name</th>
			<th>Type</th>
			<th>Description</th>
		</tr>
		<xsl:apply-templates select="function">
			<xsl:sort select="@name"></xsl:sort>
		</xsl:apply-templates>
	</table>
	
</div>
</xsl:template>

<xsl:template match="function[@invoke_kind='propertyget' or @invoke_kind='propertyput']">
	<xsl:variable name="same-name" select="../function[@name=current()/@name]"></xsl:variable>
	<xsl:if test="generate-id($same-name[1])=generate-id()">
		<tr class="{generate-id()} {generate-id($same-name[1])}">
			<td><span class="img {@invoke_kind}"> </span></td>
			<td><xsl:value-of select="@name" /></td>
			<td><xsl:apply-templates select="." mode="type" /></td>
			<td>
				<xsl:apply-templates select="$same-name/@invoke_kind"/>
				<xsl:text> </xsl:text>
				<xsl:apply-templates select="documentation" />
			</td>
		</tr>
	</xsl:if>
</xsl:template>

<xsl:template match="@invoke_kind[.='propertyget']">
	<xsl:text>Gets</xsl:text>
	<xsl:if test="position()!=last()"> or </xsl:if>
</xsl:template>

<xsl:template match="@invoke_kind[.='propertyput']">
	<xsl:text>Sets</xsl:text>
	<xsl:if test="position()!=last()"> or </xsl:if>
</xsl:template>

<xsl:template match="function">
	<tr>
		<td><span class="img {@invoke_kind}"> </span></td>
		<td>
			<xsl:apply-templates select="." mode="param-names" />
		</td>
		<td>
			<xsl:apply-templates select="param[in]/@typedesc" mode="input"/>
			
			<xsl:apply-templates select="param[out]/@typedesc" mode="output"/>
			
			<xsl:if test="not(param[out]) or count(param[out])!=1">
				<div class="empty"></div>
			</xsl:if>	

		</td>
		<td><xsl:apply-templates select="documentation" /></td>
	</tr>
</xsl:template>

<xsl:template match="function[count(param[out])&gt;1]" mode="param-names">
	<div>
		<xsl:value-of select="@name" /><xsl:text>(</xsl:text>
	</div>
	
	<xsl:apply-templates select="param/@name" mode="input" />
	
	<div>)</div>
</xsl:template>

<xsl:template match="function[not(param) or (count(param)=1 and param[out])]" mode="param-names">
	<div>
		<xsl:value-of select="@name" /><xsl:text>()</xsl:text>
	</div>
</xsl:template>

<xsl:template match="function[count(param[out])&lt;=1 and param[in]]" mode="param-names">
	<div>
		<xsl:value-of select="@name" /><xsl:text>(</xsl:text>
	</div>
	
	<xsl:apply-templates select="param[in]/@name" mode="input" />
	
	<div>)</div>
</xsl:template>


<xsl:template match="@name" mode="input">
	<div class="input"><xsl:value-of select="." /></div>
</xsl:template>

<xsl:template match="@name" mode="output">
	<div class="input"><xsl:value-of select="." /></div>
</xsl:template>

<xsl:template match="@typedesc" mode="output">
	<div><xsl:apply-templates select="."/></div>
</xsl:template>

<xsl:template match="@typedesc" mode="input">
	<xsl:if test="position()=1">
		<div class="empty"></div>
	</xsl:if>
	
	<div><xsl:apply-templates select="."/></div>
</xsl:template>

<xsl:template match="documentation">
	<xsl:copy-of select="." />
</xsl:template>

<xsl:template match="function" mode="type">
	<xsl:variable name="type-name" select="param[out|in]/@typedesc"></xsl:variable>
	<xsl:apply-templates select="param[out|in]/@typedesc"/>
</xsl:template>

<xsl:template match="@typedesc">
	<xsl:choose>
		<xsl:when test="//type[@name = current()]">
			<a href="#{generate-id(//type[@name = current()][1])}"><xsl:value-of select="current()" /></a>
		</xsl:when>
		<xsl:otherwise>
			<xsl:value-of select="current()" />
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template match="impl">

	<xsl:choose>
		<xsl:when test="//type[@name = current()/@name]">
			<a href="#{generate-id(//type[@name = current()/@name][1])}"><xsl:value-of select="@name" /></a>
		</xsl:when>
		<xsl:otherwise>
			<xsl:value-of select="@name" />
		</xsl:otherwise>
	</xsl:choose>
	
	<xsl:if test="position() !=1">, </xsl:if>
</xsl:template>

</xsl:stylesheet>
