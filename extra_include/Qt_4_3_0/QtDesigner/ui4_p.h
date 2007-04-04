/****************************************************************************
**
** Copyright (C) 1992-2007 Trolltech ASA. All rights reserved.
**
** This file is part of the Qt Designer of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of Qt Designer.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef UI4_H
#define UI4_H

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QStringList>
class QDomDocument;
class QDomElement;

#include <QtCore/qglobal.h>

#define QDESIGNER_UILIB_EXTERN Q_DECL_EXPORT
#define QDESIGNER_UILIB_IMPORT Q_DECL_IMPORT

#ifdef QT_DESIGNER_STATIC
#  define QDESIGNER_UILIB_EXPORT
#elif defined(QDESIGNER_UILIB_LIBRARY)
#  define QDESIGNER_UILIB_EXPORT QDESIGNER_UILIB_EXTERN
#else
#  define QDESIGNER_UILIB_EXPORT QDESIGNER_UILIB_IMPORT
#endif

#ifndef QDESIGNER_UILIB_EXPORT
#    define QDESIGNER_UILIB_EXPORT
#endif

#ifdef QFORMINTERNAL_NAMESPACE
namespace QFormInternal
{
#endif


/*******************************************************************************
** Forward declarations
*/

class DomUI;
class DomIncludes;
class DomInclude;
class DomResources;
class DomResource;
class DomActionGroup;
class DomAction;
class DomActionRef;
class DomImages;
class DomImage;
class DomImageData;
class DomCustomWidgets;
class DomHeader;
class DomCustomWidget;
class DomProperties;
class DomPropertyData;
class DomSizePolicyData;
class DomLayoutDefault;
class DomLayoutFunction;
class DomTabStops;
class DomLayout;
class DomLayoutItem;
class DomRow;
class DomColumn;
class DomItem;
class DomWidget;
class DomSpacer;
class DomColor;
class DomGradientStop;
class DomGradient;
class DomBrush;
class DomColorRole;
class DomColorGroup;
class DomPalette;
class DomFont;
class DomPoint;
class DomRect;
class DomLocale;
class DomSizePolicy;
class DomSize;
class DomDate;
class DomTime;
class DomDateTime;
class DomStringList;
class DomResourcePixmap;
class DomString;
class DomPointF;
class DomRectF;
class DomSizeF;
class DomChar;
class DomUrl;
class DomProperty;
class DomConnections;
class DomConnection;
class DomConnectionHints;
class DomConnectionHint;
class DomScript;
class DomWidgetData;
class DomDesignerData;

/*******************************************************************************
** Declarations
*/

class QDESIGNER_UILIB_EXPORT DomUI {
public:
    DomUI();
    ~DomUI();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeVersion() const { return m_has_attr_version; }
    inline QString attributeVersion() const { return m_attr_version; }
    inline void setAttributeVersion(const QString& a) { m_attr_version = a; m_has_attr_version = true; }
    inline void clearAttributeVersion() { m_has_attr_version = false; }

    inline bool hasAttributeLanguage() const { return m_has_attr_language; }
    inline QString attributeLanguage() const { return m_attr_language; }
    inline void setAttributeLanguage(const QString& a) { m_attr_language = a; m_has_attr_language = true; }
    inline void clearAttributeLanguage() { m_has_attr_language = false; }

    inline bool hasAttributeStdSetDef() const { return m_has_attr_stdSetDef; }
    inline int attributeStdSetDef() const { return m_attr_stdSetDef; }
    inline void setAttributeStdSetDef(int a) { m_attr_stdSetDef = a; m_has_attr_stdSetDef = true; }
    inline void clearAttributeStdSetDef() { m_has_attr_stdSetDef = false; }

    // child element accessors
    inline QString elementAuthor() const { return m_author; }
    void setElementAuthor(const QString& a);
    inline bool hasElementAuthor() const { return m_children & Author; }
    void clearElementAuthor();

    inline QString elementComment() const { return m_comment; }
    void setElementComment(const QString& a);
    inline bool hasElementComment() const { return m_children & Comment; }
    void clearElementComment();

    inline QString elementExportMacro() const { return m_exportMacro; }
    void setElementExportMacro(const QString& a);
    inline bool hasElementExportMacro() const { return m_children & ExportMacro; }
    void clearElementExportMacro();

    inline QString elementClass() const { return m_class; }
    void setElementClass(const QString& a);
    inline bool hasElementClass() const { return m_children & Class; }
    void clearElementClass();

    inline DomWidget* elementWidget() const { return m_widget; }
    void setElementWidget(DomWidget* a);
    inline bool hasElementWidget() const { return m_children & Widget; }
    void clearElementWidget();

    inline DomLayoutDefault* elementLayoutDefault() const { return m_layoutDefault; }
    void setElementLayoutDefault(DomLayoutDefault* a);
    inline bool hasElementLayoutDefault() const { return m_children & LayoutDefault; }
    void clearElementLayoutDefault();

    inline DomLayoutFunction* elementLayoutFunction() const { return m_layoutFunction; }
    void setElementLayoutFunction(DomLayoutFunction* a);
    inline bool hasElementLayoutFunction() const { return m_children & LayoutFunction; }
    void clearElementLayoutFunction();

    inline QString elementPixmapFunction() const { return m_pixmapFunction; }
    void setElementPixmapFunction(const QString& a);
    inline bool hasElementPixmapFunction() const { return m_children & PixmapFunction; }
    void clearElementPixmapFunction();

    inline DomCustomWidgets* elementCustomWidgets() const { return m_customWidgets; }
    void setElementCustomWidgets(DomCustomWidgets* a);
    inline bool hasElementCustomWidgets() const { return m_children & CustomWidgets; }
    void clearElementCustomWidgets();

    inline DomTabStops* elementTabStops() const { return m_tabStops; }
    void setElementTabStops(DomTabStops* a);
    inline bool hasElementTabStops() const { return m_children & TabStops; }
    void clearElementTabStops();

    inline DomImages* elementImages() const { return m_images; }
    void setElementImages(DomImages* a);
    inline bool hasElementImages() const { return m_children & Images; }
    void clearElementImages();

    inline DomIncludes* elementIncludes() const { return m_includes; }
    void setElementIncludes(DomIncludes* a);
    inline bool hasElementIncludes() const { return m_children & Includes; }
    void clearElementIncludes();

    inline DomResources* elementResources() const { return m_resources; }
    void setElementResources(DomResources* a);
    inline bool hasElementResources() const { return m_children & Resources; }
    void clearElementResources();

    inline DomConnections* elementConnections() const { return m_connections; }
    void setElementConnections(DomConnections* a);
    inline bool hasElementConnections() const { return m_children & Connections; }
    void clearElementConnections();

    inline DomDesignerData* elementDesignerdata() const { return m_designerdata; }
    void setElementDesignerdata(DomDesignerData* a);
    inline bool hasElementDesignerdata() const { return m_children & Designerdata; }
    void clearElementDesignerdata();

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_version;
    bool m_has_attr_version;

    QString m_attr_language;
    bool m_has_attr_language;

    int m_attr_stdSetDef;
    bool m_has_attr_stdSetDef;

    // child element data
    uint m_children;
    QString m_author;
    QString m_comment;
    QString m_exportMacro;
    QString m_class;
    DomWidget* m_widget;
    DomLayoutDefault* m_layoutDefault;
    DomLayoutFunction* m_layoutFunction;
    QString m_pixmapFunction;
    DomCustomWidgets* m_customWidgets;
    DomTabStops* m_tabStops;
    DomImages* m_images;
    DomIncludes* m_includes;
    DomResources* m_resources;
    DomConnections* m_connections;
    DomDesignerData* m_designerdata;
    enum Child {
        Author = 1,
        Comment = 2,
        ExportMacro = 4,
        Class = 8,
        Widget = 16,
        LayoutDefault = 32,
        LayoutFunction = 64,
        PixmapFunction = 128,
        CustomWidgets = 256,
        TabStops = 512,
        Images = 1024,
        Includes = 2048,
        Resources = 4096,
        Connections = 8192,
        Designerdata = 16384
    };

    DomUI(const DomUI &other);
    void operator = (const DomUI&other);
};

class QDESIGNER_UILIB_EXPORT DomIncludes {
public:
    DomIncludes();
    ~DomIncludes();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline QList<DomInclude*> elementInclude() const { return m_include; }
    void setElementInclude(const QList<DomInclude*>& a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    QList<DomInclude*> m_include;

    DomIncludes(const DomIncludes &other);
    void operator = (const DomIncludes&other);
};

class QDESIGNER_UILIB_EXPORT DomInclude {
public:
    DomInclude();
    ~DomInclude();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeLocation() const { return m_has_attr_location; }
    inline QString attributeLocation() const { return m_attr_location; }
    inline void setAttributeLocation(const QString& a) { m_attr_location = a; m_has_attr_location = true; }
    inline void clearAttributeLocation() { m_has_attr_location = false; }

    inline bool hasAttributeImpldecl() const { return m_has_attr_impldecl; }
    inline QString attributeImpldecl() const { return m_attr_impldecl; }
    inline void setAttributeImpldecl(const QString& a) { m_attr_impldecl = a; m_has_attr_impldecl = true; }
    inline void clearAttributeImpldecl() { m_has_attr_impldecl = false; }

    // child element accessors
private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_location;
    bool m_has_attr_location;

    QString m_attr_impldecl;
    bool m_has_attr_impldecl;

    // child element data

    DomInclude(const DomInclude &other);
    void operator = (const DomInclude&other);
};

class QDESIGNER_UILIB_EXPORT DomResources {
public:
    DomResources();
    ~DomResources();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeName() const { return m_has_attr_name; }
    inline QString attributeName() const { return m_attr_name; }
    inline void setAttributeName(const QString& a) { m_attr_name = a; m_has_attr_name = true; }
    inline void clearAttributeName() { m_has_attr_name = false; }

    // child element accessors
    inline QList<DomResource*> elementInclude() const { return m_include; }
    void setElementInclude(const QList<DomResource*>& a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_name;
    bool m_has_attr_name;

    // child element data
    QList<DomResource*> m_include;

    DomResources(const DomResources &other);
    void operator = (const DomResources&other);
};

class QDESIGNER_UILIB_EXPORT DomResource {
public:
    DomResource();
    ~DomResource();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeLocation() const { return m_has_attr_location; }
    inline QString attributeLocation() const { return m_attr_location; }
    inline void setAttributeLocation(const QString& a) { m_attr_location = a; m_has_attr_location = true; }
    inline void clearAttributeLocation() { m_has_attr_location = false; }

    // child element accessors
private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_location;
    bool m_has_attr_location;

    // child element data

    DomResource(const DomResource &other);
    void operator = (const DomResource&other);
};

class QDESIGNER_UILIB_EXPORT DomActionGroup {
public:
    DomActionGroup();
    ~DomActionGroup();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeName() const { return m_has_attr_name; }
    inline QString attributeName() const { return m_attr_name; }
    inline void setAttributeName(const QString& a) { m_attr_name = a; m_has_attr_name = true; }
    inline void clearAttributeName() { m_has_attr_name = false; }

    // child element accessors
    inline QList<DomAction*> elementAction() const { return m_action; }
    void setElementAction(const QList<DomAction*>& a);

    inline QList<DomActionGroup*> elementActionGroup() const { return m_actionGroup; }
    void setElementActionGroup(const QList<DomActionGroup*>& a);

    inline QList<DomProperty*> elementProperty() const { return m_property; }
    void setElementProperty(const QList<DomProperty*>& a);

    inline QList<DomProperty*> elementAttribute() const { return m_attribute; }
    void setElementAttribute(const QList<DomProperty*>& a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_name;
    bool m_has_attr_name;

    // child element data
    QList<DomAction*> m_action;
    QList<DomActionGroup*> m_actionGroup;
    QList<DomProperty*> m_property;
    QList<DomProperty*> m_attribute;

    DomActionGroup(const DomActionGroup &other);
    void operator = (const DomActionGroup&other);
};

class QDESIGNER_UILIB_EXPORT DomAction {
public:
    DomAction();
    ~DomAction();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeName() const { return m_has_attr_name; }
    inline QString attributeName() const { return m_attr_name; }
    inline void setAttributeName(const QString& a) { m_attr_name = a; m_has_attr_name = true; }
    inline void clearAttributeName() { m_has_attr_name = false; }

    inline bool hasAttributeMenu() const { return m_has_attr_menu; }
    inline QString attributeMenu() const { return m_attr_menu; }
    inline void setAttributeMenu(const QString& a) { m_attr_menu = a; m_has_attr_menu = true; }
    inline void clearAttributeMenu() { m_has_attr_menu = false; }

    // child element accessors
    inline QList<DomProperty*> elementProperty() const { return m_property; }
    void setElementProperty(const QList<DomProperty*>& a);

    inline QList<DomProperty*> elementAttribute() const { return m_attribute; }
    void setElementAttribute(const QList<DomProperty*>& a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_name;
    bool m_has_attr_name;

    QString m_attr_menu;
    bool m_has_attr_menu;

    // child element data
    QList<DomProperty*> m_property;
    QList<DomProperty*> m_attribute;

    DomAction(const DomAction &other);
    void operator = (const DomAction&other);
};

class QDESIGNER_UILIB_EXPORT DomActionRef {
public:
    DomActionRef();
    ~DomActionRef();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeName() const { return m_has_attr_name; }
    inline QString attributeName() const { return m_attr_name; }
    inline void setAttributeName(const QString& a) { m_attr_name = a; m_has_attr_name = true; }
    inline void clearAttributeName() { m_has_attr_name = false; }

    // child element accessors
private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_name;
    bool m_has_attr_name;

    // child element data

    DomActionRef(const DomActionRef &other);
    void operator = (const DomActionRef&other);
};

class QDESIGNER_UILIB_EXPORT DomImages {
public:
    DomImages();
    ~DomImages();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline QList<DomImage*> elementImage() const { return m_image; }
    void setElementImage(const QList<DomImage*>& a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    QList<DomImage*> m_image;

    DomImages(const DomImages &other);
    void operator = (const DomImages&other);
};

class QDESIGNER_UILIB_EXPORT DomImage {
public:
    DomImage();
    ~DomImage();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeName() const { return m_has_attr_name; }
    inline QString attributeName() const { return m_attr_name; }
    inline void setAttributeName(const QString& a) { m_attr_name = a; m_has_attr_name = true; }
    inline void clearAttributeName() { m_has_attr_name = false; }

    // child element accessors
    inline DomImageData* elementData() const { return m_data; }
    void setElementData(DomImageData* a);
    inline bool hasElementData() const { return m_children & Data; }
    void clearElementData();

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_name;
    bool m_has_attr_name;

    // child element data
    uint m_children;
    DomImageData* m_data;
    enum Child {
        Data = 1
    };

    DomImage(const DomImage &other);
    void operator = (const DomImage&other);
};

class QDESIGNER_UILIB_EXPORT DomImageData {
public:
    DomImageData();
    ~DomImageData();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeFormat() const { return m_has_attr_format; }
    inline QString attributeFormat() const { return m_attr_format; }
    inline void setAttributeFormat(const QString& a) { m_attr_format = a; m_has_attr_format = true; }
    inline void clearAttributeFormat() { m_has_attr_format = false; }

    inline bool hasAttributeLength() const { return m_has_attr_length; }
    inline int attributeLength() const { return m_attr_length; }
    inline void setAttributeLength(int a) { m_attr_length = a; m_has_attr_length = true; }
    inline void clearAttributeLength() { m_has_attr_length = false; }

    // child element accessors
private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_format;
    bool m_has_attr_format;

    int m_attr_length;
    bool m_has_attr_length;

    // child element data

    DomImageData(const DomImageData &other);
    void operator = (const DomImageData&other);
};

class QDESIGNER_UILIB_EXPORT DomCustomWidgets {
public:
    DomCustomWidgets();
    ~DomCustomWidgets();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline QList<DomCustomWidget*> elementCustomWidget() const { return m_customWidget; }
    void setElementCustomWidget(const QList<DomCustomWidget*>& a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    QList<DomCustomWidget*> m_customWidget;

    DomCustomWidgets(const DomCustomWidgets &other);
    void operator = (const DomCustomWidgets&other);
};

class QDESIGNER_UILIB_EXPORT DomHeader {
public:
    DomHeader();
    ~DomHeader();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeLocation() const { return m_has_attr_location; }
    inline QString attributeLocation() const { return m_attr_location; }
    inline void setAttributeLocation(const QString& a) { m_attr_location = a; m_has_attr_location = true; }
    inline void clearAttributeLocation() { m_has_attr_location = false; }

    // child element accessors
private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_location;
    bool m_has_attr_location;

    // child element data

    DomHeader(const DomHeader &other);
    void operator = (const DomHeader&other);
};

class QDESIGNER_UILIB_EXPORT DomCustomWidget {
public:
    DomCustomWidget();
    ~DomCustomWidget();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline QString elementClass() const { return m_class; }
    void setElementClass(const QString& a);
    inline bool hasElementClass() const { return m_children & Class; }
    void clearElementClass();

    inline QString elementExtends() const { return m_extends; }
    void setElementExtends(const QString& a);
    inline bool hasElementExtends() const { return m_children & Extends; }
    void clearElementExtends();

    inline DomHeader* elementHeader() const { return m_header; }
    void setElementHeader(DomHeader* a);
    inline bool hasElementHeader() const { return m_children & Header; }
    void clearElementHeader();

    inline DomSize* elementSizeHint() const { return m_sizeHint; }
    void setElementSizeHint(DomSize* a);
    inline bool hasElementSizeHint() const { return m_children & SizeHint; }
    void clearElementSizeHint();

    inline int elementContainer() const { return m_container; }
    void setElementContainer(int a);
    inline bool hasElementContainer() const { return m_children & Container; }
    void clearElementContainer();

    inline DomSizePolicyData* elementSizePolicy() const { return m_sizePolicy; }
    void setElementSizePolicy(DomSizePolicyData* a);
    inline bool hasElementSizePolicy() const { return m_children & SizePolicy; }
    void clearElementSizePolicy();

    inline QString elementPixmap() const { return m_pixmap; }
    void setElementPixmap(const QString& a);
    inline bool hasElementPixmap() const { return m_children & Pixmap; }
    void clearElementPixmap();

    inline DomScript* elementScript() const { return m_script; }
    void setElementScript(DomScript* a);
    inline bool hasElementScript() const { return m_children & Script; }
    void clearElementScript();

    inline DomProperties* elementProperties() const { return m_properties; }
    void setElementProperties(DomProperties* a);
    inline bool hasElementProperties() const { return m_children & Properties; }
    void clearElementProperties();

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    uint m_children;
    QString m_class;
    QString m_extends;
    DomHeader* m_header;
    DomSize* m_sizeHint;
    int m_container;
    DomSizePolicyData* m_sizePolicy;
    QString m_pixmap;
    DomScript* m_script;
    DomProperties* m_properties;
    enum Child {
        Class = 1,
        Extends = 2,
        Header = 4,
        SizeHint = 8,
        Container = 16,
        SizePolicy = 32,
        Pixmap = 64,
        Script = 128,
        Properties = 256
    };

    DomCustomWidget(const DomCustomWidget &other);
    void operator = (const DomCustomWidget&other);
};

class QDESIGNER_UILIB_EXPORT DomProperties {
public:
    DomProperties();
    ~DomProperties();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline QList<DomPropertyData*> elementProperty() const { return m_property; }
    void setElementProperty(const QList<DomPropertyData*>& a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    QList<DomPropertyData*> m_property;

    DomProperties(const DomProperties &other);
    void operator = (const DomProperties&other);
};

class QDESIGNER_UILIB_EXPORT DomPropertyData {
public:
    DomPropertyData();
    ~DomPropertyData();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeType() const { return m_has_attr_type; }
    inline QString attributeType() const { return m_attr_type; }
    inline void setAttributeType(const QString& a) { m_attr_type = a; m_has_attr_type = true; }
    inline void clearAttributeType() { m_has_attr_type = false; }

    // child element accessors
private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_type;
    bool m_has_attr_type;

    // child element data

    DomPropertyData(const DomPropertyData &other);
    void operator = (const DomPropertyData&other);
};

class QDESIGNER_UILIB_EXPORT DomSizePolicyData {
public:
    DomSizePolicyData();
    ~DomSizePolicyData();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline int elementHorData() const { return m_horData; }
    void setElementHorData(int a);
    inline bool hasElementHorData() const { return m_children & HorData; }
    void clearElementHorData();

    inline int elementVerData() const { return m_verData; }
    void setElementVerData(int a);
    inline bool hasElementVerData() const { return m_children & VerData; }
    void clearElementVerData();

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    uint m_children;
    int m_horData;
    int m_verData;
    enum Child {
        HorData = 1,
        VerData = 2
    };

    DomSizePolicyData(const DomSizePolicyData &other);
    void operator = (const DomSizePolicyData&other);
};

class QDESIGNER_UILIB_EXPORT DomLayoutDefault {
public:
    DomLayoutDefault();
    ~DomLayoutDefault();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeSpacing() const { return m_has_attr_spacing; }
    inline int attributeSpacing() const { return m_attr_spacing; }
    inline void setAttributeSpacing(int a) { m_attr_spacing = a; m_has_attr_spacing = true; }
    inline void clearAttributeSpacing() { m_has_attr_spacing = false; }

    inline bool hasAttributeMargin() const { return m_has_attr_margin; }
    inline int attributeMargin() const { return m_attr_margin; }
    inline void setAttributeMargin(int a) { m_attr_margin = a; m_has_attr_margin = true; }
    inline void clearAttributeMargin() { m_has_attr_margin = false; }

    // child element accessors
private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    int m_attr_spacing;
    bool m_has_attr_spacing;

    int m_attr_margin;
    bool m_has_attr_margin;

    // child element data

    DomLayoutDefault(const DomLayoutDefault &other);
    void operator = (const DomLayoutDefault&other);
};

class QDESIGNER_UILIB_EXPORT DomLayoutFunction {
public:
    DomLayoutFunction();
    ~DomLayoutFunction();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeSpacing() const { return m_has_attr_spacing; }
    inline QString attributeSpacing() const { return m_attr_spacing; }
    inline void setAttributeSpacing(const QString& a) { m_attr_spacing = a; m_has_attr_spacing = true; }
    inline void clearAttributeSpacing() { m_has_attr_spacing = false; }

    inline bool hasAttributeMargin() const { return m_has_attr_margin; }
    inline QString attributeMargin() const { return m_attr_margin; }
    inline void setAttributeMargin(const QString& a) { m_attr_margin = a; m_has_attr_margin = true; }
    inline void clearAttributeMargin() { m_has_attr_margin = false; }

    // child element accessors
private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_spacing;
    bool m_has_attr_spacing;

    QString m_attr_margin;
    bool m_has_attr_margin;

    // child element data

    DomLayoutFunction(const DomLayoutFunction &other);
    void operator = (const DomLayoutFunction&other);
};

class QDESIGNER_UILIB_EXPORT DomTabStops {
public:
    DomTabStops();
    ~DomTabStops();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline QStringList elementTabStop() const { return m_tabStop; }
    void setElementTabStop(const QStringList& a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    QStringList m_tabStop;

    DomTabStops(const DomTabStops &other);
    void operator = (const DomTabStops&other);
};

class QDESIGNER_UILIB_EXPORT DomLayout {
public:
    DomLayout();
    ~DomLayout();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeClass() const { return m_has_attr_class; }
    inline QString attributeClass() const { return m_attr_class; }
    inline void setAttributeClass(const QString& a) { m_attr_class = a; m_has_attr_class = true; }
    inline void clearAttributeClass() { m_has_attr_class = false; }

    // child element accessors
    inline QList<DomProperty*> elementProperty() const { return m_property; }
    void setElementProperty(const QList<DomProperty*>& a);

    inline QList<DomProperty*> elementAttribute() const { return m_attribute; }
    void setElementAttribute(const QList<DomProperty*>& a);

    inline QList<DomLayoutItem*> elementItem() const { return m_item; }
    void setElementItem(const QList<DomLayoutItem*>& a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_class;
    bool m_has_attr_class;

    // child element data
    QList<DomProperty*> m_property;
    QList<DomProperty*> m_attribute;
    QList<DomLayoutItem*> m_item;

    DomLayout(const DomLayout &other);
    void operator = (const DomLayout&other);
};

class QDESIGNER_UILIB_EXPORT DomLayoutItem {
public:
    DomLayoutItem();
    ~DomLayoutItem();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeRow() const { return m_has_attr_row; }
    inline int attributeRow() const { return m_attr_row; }
    inline void setAttributeRow(int a) { m_attr_row = a; m_has_attr_row = true; }
    inline void clearAttributeRow() { m_has_attr_row = false; }

    inline bool hasAttributeColumn() const { return m_has_attr_column; }
    inline int attributeColumn() const { return m_attr_column; }
    inline void setAttributeColumn(int a) { m_attr_column = a; m_has_attr_column = true; }
    inline void clearAttributeColumn() { m_has_attr_column = false; }

    inline bool hasAttributeRowSpan() const { return m_has_attr_rowSpan; }
    inline int attributeRowSpan() const { return m_attr_rowSpan; }
    inline void setAttributeRowSpan(int a) { m_attr_rowSpan = a; m_has_attr_rowSpan = true; }
    inline void clearAttributeRowSpan() { m_has_attr_rowSpan = false; }

    inline bool hasAttributeColSpan() const { return m_has_attr_colSpan; }
    inline int attributeColSpan() const { return m_attr_colSpan; }
    inline void setAttributeColSpan(int a) { m_attr_colSpan = a; m_has_attr_colSpan = true; }
    inline void clearAttributeColSpan() { m_has_attr_colSpan = false; }

    // child element accessors
    enum Kind { Unknown = 0, Widget, Layout, Spacer };
    inline Kind kind() const { return m_kind; }

    inline DomWidget* elementWidget() const { return m_widget; }
    void setElementWidget(DomWidget* a);

    inline DomLayout* elementLayout() const { return m_layout; }
    void setElementLayout(DomLayout* a);

    inline DomSpacer* elementSpacer() const { return m_spacer; }
    void setElementSpacer(DomSpacer* a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    int m_attr_row;
    bool m_has_attr_row;

    int m_attr_column;
    bool m_has_attr_column;

    int m_attr_rowSpan;
    bool m_has_attr_rowSpan;

    int m_attr_colSpan;
    bool m_has_attr_colSpan;

    // child element data
    Kind m_kind;
    DomWidget* m_widget;
    DomLayout* m_layout;
    DomSpacer* m_spacer;

    DomLayoutItem(const DomLayoutItem &other);
    void operator = (const DomLayoutItem&other);
};

class QDESIGNER_UILIB_EXPORT DomRow {
public:
    DomRow();
    ~DomRow();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline QList<DomProperty*> elementProperty() const { return m_property; }
    void setElementProperty(const QList<DomProperty*>& a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    QList<DomProperty*> m_property;

    DomRow(const DomRow &other);
    void operator = (const DomRow&other);
};

class QDESIGNER_UILIB_EXPORT DomColumn {
public:
    DomColumn();
    ~DomColumn();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline QList<DomProperty*> elementProperty() const { return m_property; }
    void setElementProperty(const QList<DomProperty*>& a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    QList<DomProperty*> m_property;

    DomColumn(const DomColumn &other);
    void operator = (const DomColumn&other);
};

class QDESIGNER_UILIB_EXPORT DomItem {
public:
    DomItem();
    ~DomItem();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeRow() const { return m_has_attr_row; }
    inline int attributeRow() const { return m_attr_row; }
    inline void setAttributeRow(int a) { m_attr_row = a; m_has_attr_row = true; }
    inline void clearAttributeRow() { m_has_attr_row = false; }

    inline bool hasAttributeColumn() const { return m_has_attr_column; }
    inline int attributeColumn() const { return m_attr_column; }
    inline void setAttributeColumn(int a) { m_attr_column = a; m_has_attr_column = true; }
    inline void clearAttributeColumn() { m_has_attr_column = false; }

    // child element accessors
    inline QList<DomProperty*> elementProperty() const { return m_property; }
    void setElementProperty(const QList<DomProperty*>& a);

    inline QList<DomItem*> elementItem() const { return m_item; }
    void setElementItem(const QList<DomItem*>& a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    int m_attr_row;
    bool m_has_attr_row;

    int m_attr_column;
    bool m_has_attr_column;

    // child element data
    QList<DomProperty*> m_property;
    QList<DomItem*> m_item;

    DomItem(const DomItem &other);
    void operator = (const DomItem&other);
};

class QDESIGNER_UILIB_EXPORT DomWidget {
public:
    DomWidget();
    ~DomWidget();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeClass() const { return m_has_attr_class; }
    inline QString attributeClass() const { return m_attr_class; }
    inline void setAttributeClass(const QString& a) { m_attr_class = a; m_has_attr_class = true; }
    inline void clearAttributeClass() { m_has_attr_class = false; }

    inline bool hasAttributeName() const { return m_has_attr_name; }
    inline QString attributeName() const { return m_attr_name; }
    inline void setAttributeName(const QString& a) { m_attr_name = a; m_has_attr_name = true; }
    inline void clearAttributeName() { m_has_attr_name = false; }

    inline bool hasAttributeNative() const { return m_has_attr_native; }
    inline bool attributeNative() const { return m_attr_native; }
    inline void setAttributeNative(bool a) { m_attr_native = a; m_has_attr_native = true; }
    inline void clearAttributeNative() { m_has_attr_native = false; }

    // child element accessors
    inline QStringList elementClass() const { return m_class; }
    void setElementClass(const QStringList& a);

    inline QList<DomProperty*> elementProperty() const { return m_property; }
    void setElementProperty(const QList<DomProperty*>& a);

    inline QList<DomScript*> elementScript() const { return m_script; }
    void setElementScript(const QList<DomScript*>& a);

    inline QList<DomWidgetData*> elementWidgetData() const { return m_widgetData; }
    void setElementWidgetData(const QList<DomWidgetData*>& a);

    inline QList<DomProperty*> elementAttribute() const { return m_attribute; }
    void setElementAttribute(const QList<DomProperty*>& a);

    inline QList<DomRow*> elementRow() const { return m_row; }
    void setElementRow(const QList<DomRow*>& a);

    inline QList<DomColumn*> elementColumn() const { return m_column; }
    void setElementColumn(const QList<DomColumn*>& a);

    inline QList<DomItem*> elementItem() const { return m_item; }
    void setElementItem(const QList<DomItem*>& a);

    inline QList<DomLayout*> elementLayout() const { return m_layout; }
    void setElementLayout(const QList<DomLayout*>& a);

    inline QList<DomWidget*> elementWidget() const { return m_widget; }
    void setElementWidget(const QList<DomWidget*>& a);

    inline QList<DomAction*> elementAction() const { return m_action; }
    void setElementAction(const QList<DomAction*>& a);

    inline QList<DomActionGroup*> elementActionGroup() const { return m_actionGroup; }
    void setElementActionGroup(const QList<DomActionGroup*>& a);

    inline QList<DomActionRef*> elementAddAction() const { return m_addAction; }
    void setElementAddAction(const QList<DomActionRef*>& a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_class;
    bool m_has_attr_class;

    QString m_attr_name;
    bool m_has_attr_name;

    bool m_attr_native;
    bool m_has_attr_native;

    // child element data
    QStringList m_class;
    QList<DomProperty*> m_property;
    QList<DomScript*> m_script;
    QList<DomWidgetData*> m_widgetData;
    QList<DomProperty*> m_attribute;
    QList<DomRow*> m_row;
    QList<DomColumn*> m_column;
    QList<DomItem*> m_item;
    QList<DomLayout*> m_layout;
    QList<DomWidget*> m_widget;
    QList<DomAction*> m_action;
    QList<DomActionGroup*> m_actionGroup;
    QList<DomActionRef*> m_addAction;

    DomWidget(const DomWidget &other);
    void operator = (const DomWidget&other);
};

class QDESIGNER_UILIB_EXPORT DomSpacer {
public:
    DomSpacer();
    ~DomSpacer();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeName() const { return m_has_attr_name; }
    inline QString attributeName() const { return m_attr_name; }
    inline void setAttributeName(const QString& a) { m_attr_name = a; m_has_attr_name = true; }
    inline void clearAttributeName() { m_has_attr_name = false; }

    // child element accessors
    inline QList<DomProperty*> elementProperty() const { return m_property; }
    void setElementProperty(const QList<DomProperty*>& a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_name;
    bool m_has_attr_name;

    // child element data
    QList<DomProperty*> m_property;

    DomSpacer(const DomSpacer &other);
    void operator = (const DomSpacer&other);
};

class QDESIGNER_UILIB_EXPORT DomColor {
public:
    DomColor();
    ~DomColor();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeAlpha() const { return m_has_attr_alpha; }
    inline int attributeAlpha() const { return m_attr_alpha; }
    inline void setAttributeAlpha(int a) { m_attr_alpha = a; m_has_attr_alpha = true; }
    inline void clearAttributeAlpha() { m_has_attr_alpha = false; }

    // child element accessors
    inline int elementRed() const { return m_red; }
    void setElementRed(int a);
    inline bool hasElementRed() const { return m_children & Red; }
    void clearElementRed();

    inline int elementGreen() const { return m_green; }
    void setElementGreen(int a);
    inline bool hasElementGreen() const { return m_children & Green; }
    void clearElementGreen();

    inline int elementBlue() const { return m_blue; }
    void setElementBlue(int a);
    inline bool hasElementBlue() const { return m_children & Blue; }
    void clearElementBlue();

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    int m_attr_alpha;
    bool m_has_attr_alpha;

    // child element data
    uint m_children;
    int m_red;
    int m_green;
    int m_blue;
    enum Child {
        Red = 1,
        Green = 2,
        Blue = 4
    };

    DomColor(const DomColor &other);
    void operator = (const DomColor&other);
};

class QDESIGNER_UILIB_EXPORT DomGradientStop {
public:
    DomGradientStop();
    ~DomGradientStop();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributePosition() const { return m_has_attr_position; }
    inline double attributePosition() const { return m_attr_position; }
    inline void setAttributePosition(double a) { m_attr_position = a; m_has_attr_position = true; }
    inline void clearAttributePosition() { m_has_attr_position = false; }

    // child element accessors
    inline DomColor* elementColor() const { return m_color; }
    void setElementColor(DomColor* a);
    inline bool hasElementColor() const { return m_children & Color; }
    void clearElementColor();

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    double m_attr_position;
    bool m_has_attr_position;

    // child element data
    uint m_children;
    DomColor* m_color;
    enum Child {
        Color = 1
    };

    DomGradientStop(const DomGradientStop &other);
    void operator = (const DomGradientStop&other);
};

class QDESIGNER_UILIB_EXPORT DomGradient {
public:
    DomGradient();
    ~DomGradient();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeStartX() const { return m_has_attr_startX; }
    inline double attributeStartX() const { return m_attr_startX; }
    inline void setAttributeStartX(double a) { m_attr_startX = a; m_has_attr_startX = true; }
    inline void clearAttributeStartX() { m_has_attr_startX = false; }

    inline bool hasAttributeStartY() const { return m_has_attr_startY; }
    inline double attributeStartY() const { return m_attr_startY; }
    inline void setAttributeStartY(double a) { m_attr_startY = a; m_has_attr_startY = true; }
    inline void clearAttributeStartY() { m_has_attr_startY = false; }

    inline bool hasAttributeEndX() const { return m_has_attr_endX; }
    inline double attributeEndX() const { return m_attr_endX; }
    inline void setAttributeEndX(double a) { m_attr_endX = a; m_has_attr_endX = true; }
    inline void clearAttributeEndX() { m_has_attr_endX = false; }

    inline bool hasAttributeEndY() const { return m_has_attr_endY; }
    inline double attributeEndY() const { return m_attr_endY; }
    inline void setAttributeEndY(double a) { m_attr_endY = a; m_has_attr_endY = true; }
    inline void clearAttributeEndY() { m_has_attr_endY = false; }

    inline bool hasAttributeCentralX() const { return m_has_attr_centralX; }
    inline double attributeCentralX() const { return m_attr_centralX; }
    inline void setAttributeCentralX(double a) { m_attr_centralX = a; m_has_attr_centralX = true; }
    inline void clearAttributeCentralX() { m_has_attr_centralX = false; }

    inline bool hasAttributeCentralY() const { return m_has_attr_centralY; }
    inline double attributeCentralY() const { return m_attr_centralY; }
    inline void setAttributeCentralY(double a) { m_attr_centralY = a; m_has_attr_centralY = true; }
    inline void clearAttributeCentralY() { m_has_attr_centralY = false; }

    inline bool hasAttributeFocalX() const { return m_has_attr_focalX; }
    inline double attributeFocalX() const { return m_attr_focalX; }
    inline void setAttributeFocalX(double a) { m_attr_focalX = a; m_has_attr_focalX = true; }
    inline void clearAttributeFocalX() { m_has_attr_focalX = false; }

    inline bool hasAttributeFocalY() const { return m_has_attr_focalY; }
    inline double attributeFocalY() const { return m_attr_focalY; }
    inline void setAttributeFocalY(double a) { m_attr_focalY = a; m_has_attr_focalY = true; }
    inline void clearAttributeFocalY() { m_has_attr_focalY = false; }

    inline bool hasAttributeRadius() const { return m_has_attr_radius; }
    inline double attributeRadius() const { return m_attr_radius; }
    inline void setAttributeRadius(double a) { m_attr_radius = a; m_has_attr_radius = true; }
    inline void clearAttributeRadius() { m_has_attr_radius = false; }

    inline bool hasAttributeAngle() const { return m_has_attr_angle; }
    inline double attributeAngle() const { return m_attr_angle; }
    inline void setAttributeAngle(double a) { m_attr_angle = a; m_has_attr_angle = true; }
    inline void clearAttributeAngle() { m_has_attr_angle = false; }

    inline bool hasAttributeType() const { return m_has_attr_type; }
    inline QString attributeType() const { return m_attr_type; }
    inline void setAttributeType(const QString& a) { m_attr_type = a; m_has_attr_type = true; }
    inline void clearAttributeType() { m_has_attr_type = false; }

    inline bool hasAttributeSpread() const { return m_has_attr_spread; }
    inline QString attributeSpread() const { return m_attr_spread; }
    inline void setAttributeSpread(const QString& a) { m_attr_spread = a; m_has_attr_spread = true; }
    inline void clearAttributeSpread() { m_has_attr_spread = false; }

    inline bool hasAttributeCoordinateMode() const { return m_has_attr_coordinateMode; }
    inline QString attributeCoordinateMode() const { return m_attr_coordinateMode; }
    inline void setAttributeCoordinateMode(const QString& a) { m_attr_coordinateMode = a; m_has_attr_coordinateMode = true; }
    inline void clearAttributeCoordinateMode() { m_has_attr_coordinateMode = false; }

    // child element accessors
    inline QList<DomGradientStop*> elementGradientStop() const { return m_gradientStop; }
    void setElementGradientStop(const QList<DomGradientStop*>& a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    double m_attr_startX;
    bool m_has_attr_startX;

    double m_attr_startY;
    bool m_has_attr_startY;

    double m_attr_endX;
    bool m_has_attr_endX;

    double m_attr_endY;
    bool m_has_attr_endY;

    double m_attr_centralX;
    bool m_has_attr_centralX;

    double m_attr_centralY;
    bool m_has_attr_centralY;

    double m_attr_focalX;
    bool m_has_attr_focalX;

    double m_attr_focalY;
    bool m_has_attr_focalY;

    double m_attr_radius;
    bool m_has_attr_radius;

    double m_attr_angle;
    bool m_has_attr_angle;

    QString m_attr_type;
    bool m_has_attr_type;

    QString m_attr_spread;
    bool m_has_attr_spread;

    QString m_attr_coordinateMode;
    bool m_has_attr_coordinateMode;

    // child element data
    QList<DomGradientStop*> m_gradientStop;

    DomGradient(const DomGradient &other);
    void operator = (const DomGradient&other);
};

class QDESIGNER_UILIB_EXPORT DomBrush {
public:
    DomBrush();
    ~DomBrush();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeBrushStyle() const { return m_has_attr_brushStyle; }
    inline QString attributeBrushStyle() const { return m_attr_brushStyle; }
    inline void setAttributeBrushStyle(const QString& a) { m_attr_brushStyle = a; m_has_attr_brushStyle = true; }
    inline void clearAttributeBrushStyle() { m_has_attr_brushStyle = false; }

    // child element accessors
    enum Kind { Unknown = 0, Color, Texture, Gradient };
    inline Kind kind() const { return m_kind; }

    inline DomColor* elementColor() const { return m_color; }
    void setElementColor(DomColor* a);

    inline DomProperty* elementTexture() const { return m_texture; }
    void setElementTexture(DomProperty* a);

    inline DomGradient* elementGradient() const { return m_gradient; }
    void setElementGradient(DomGradient* a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_brushStyle;
    bool m_has_attr_brushStyle;

    // child element data
    Kind m_kind;
    DomColor* m_color;
    DomProperty* m_texture;
    DomGradient* m_gradient;

    DomBrush(const DomBrush &other);
    void operator = (const DomBrush&other);
};

class QDESIGNER_UILIB_EXPORT DomColorRole {
public:
    DomColorRole();
    ~DomColorRole();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeRole() const { return m_has_attr_role; }
    inline QString attributeRole() const { return m_attr_role; }
    inline void setAttributeRole(const QString& a) { m_attr_role = a; m_has_attr_role = true; }
    inline void clearAttributeRole() { m_has_attr_role = false; }

    // child element accessors
    inline DomBrush* elementBrush() const { return m_brush; }
    void setElementBrush(DomBrush* a);
    inline bool hasElementBrush() const { return m_children & Brush; }
    void clearElementBrush();

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_role;
    bool m_has_attr_role;

    // child element data
    uint m_children;
    DomBrush* m_brush;
    enum Child {
        Brush = 1
    };

    DomColorRole(const DomColorRole &other);
    void operator = (const DomColorRole&other);
};

class QDESIGNER_UILIB_EXPORT DomColorGroup {
public:
    DomColorGroup();
    ~DomColorGroup();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline QList<DomColorRole*> elementColorRole() const { return m_colorRole; }
    void setElementColorRole(const QList<DomColorRole*>& a);

    inline QList<DomColor*> elementColor() const { return m_color; }
    void setElementColor(const QList<DomColor*>& a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    QList<DomColorRole*> m_colorRole;
    QList<DomColor*> m_color;

    DomColorGroup(const DomColorGroup &other);
    void operator = (const DomColorGroup&other);
};

class QDESIGNER_UILIB_EXPORT DomPalette {
public:
    DomPalette();
    ~DomPalette();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline DomColorGroup* elementActive() const { return m_active; }
    void setElementActive(DomColorGroup* a);
    inline bool hasElementActive() const { return m_children & Active; }
    void clearElementActive();

    inline DomColorGroup* elementInactive() const { return m_inactive; }
    void setElementInactive(DomColorGroup* a);
    inline bool hasElementInactive() const { return m_children & Inactive; }
    void clearElementInactive();

    inline DomColorGroup* elementDisabled() const { return m_disabled; }
    void setElementDisabled(DomColorGroup* a);
    inline bool hasElementDisabled() const { return m_children & Disabled; }
    void clearElementDisabled();

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    uint m_children;
    DomColorGroup* m_active;
    DomColorGroup* m_inactive;
    DomColorGroup* m_disabled;
    enum Child {
        Active = 1,
        Inactive = 2,
        Disabled = 4
    };

    DomPalette(const DomPalette &other);
    void operator = (const DomPalette&other);
};

class QDESIGNER_UILIB_EXPORT DomFont {
public:
    DomFont();
    ~DomFont();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline QString elementFamily() const { return m_family; }
    void setElementFamily(const QString& a);
    inline bool hasElementFamily() const { return m_children & Family; }
    void clearElementFamily();

    inline int elementPointSize() const { return m_pointSize; }
    void setElementPointSize(int a);
    inline bool hasElementPointSize() const { return m_children & PointSize; }
    void clearElementPointSize();

    inline int elementWeight() const { return m_weight; }
    void setElementWeight(int a);
    inline bool hasElementWeight() const { return m_children & Weight; }
    void clearElementWeight();

    inline bool elementItalic() const { return m_italic; }
    void setElementItalic(bool a);
    inline bool hasElementItalic() const { return m_children & Italic; }
    void clearElementItalic();

    inline bool elementBold() const { return m_bold; }
    void setElementBold(bool a);
    inline bool hasElementBold() const { return m_children & Bold; }
    void clearElementBold();

    inline bool elementUnderline() const { return m_underline; }
    void setElementUnderline(bool a);
    inline bool hasElementUnderline() const { return m_children & Underline; }
    void clearElementUnderline();

    inline bool elementStrikeOut() const { return m_strikeOut; }
    void setElementStrikeOut(bool a);
    inline bool hasElementStrikeOut() const { return m_children & StrikeOut; }
    void clearElementStrikeOut();

    inline bool elementAntialiasing() const { return m_antialiasing; }
    void setElementAntialiasing(bool a);
    inline bool hasElementAntialiasing() const { return m_children & Antialiasing; }
    void clearElementAntialiasing();

    inline QString elementStyleStrategy() const { return m_styleStrategy; }
    void setElementStyleStrategy(const QString& a);
    inline bool hasElementStyleStrategy() const { return m_children & StyleStrategy; }
    void clearElementStyleStrategy();

    inline bool elementKerning() const { return m_kerning; }
    void setElementKerning(bool a);
    inline bool hasElementKerning() const { return m_children & Kerning; }
    void clearElementKerning();

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    uint m_children;
    QString m_family;
    int m_pointSize;
    int m_weight;
    bool m_italic;
    bool m_bold;
    bool m_underline;
    bool m_strikeOut;
    bool m_antialiasing;
    QString m_styleStrategy;
    bool m_kerning;
    enum Child {
        Family = 1,
        PointSize = 2,
        Weight = 4,
        Italic = 8,
        Bold = 16,
        Underline = 32,
        StrikeOut = 64,
        Antialiasing = 128,
        StyleStrategy = 256,
        Kerning = 512
    };

    DomFont(const DomFont &other);
    void operator = (const DomFont&other);
};

class QDESIGNER_UILIB_EXPORT DomPoint {
public:
    DomPoint();
    ~DomPoint();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline int elementX() const { return m_x; }
    void setElementX(int a);
    inline bool hasElementX() const { return m_children & X; }
    void clearElementX();

    inline int elementY() const { return m_y; }
    void setElementY(int a);
    inline bool hasElementY() const { return m_children & Y; }
    void clearElementY();

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    uint m_children;
    int m_x;
    int m_y;
    enum Child {
        X = 1,
        Y = 2
    };

    DomPoint(const DomPoint &other);
    void operator = (const DomPoint&other);
};

class QDESIGNER_UILIB_EXPORT DomRect {
public:
    DomRect();
    ~DomRect();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline int elementX() const { return m_x; }
    void setElementX(int a);
    inline bool hasElementX() const { return m_children & X; }
    void clearElementX();

    inline int elementY() const { return m_y; }
    void setElementY(int a);
    inline bool hasElementY() const { return m_children & Y; }
    void clearElementY();

    inline int elementWidth() const { return m_width; }
    void setElementWidth(int a);
    inline bool hasElementWidth() const { return m_children & Width; }
    void clearElementWidth();

    inline int elementHeight() const { return m_height; }
    void setElementHeight(int a);
    inline bool hasElementHeight() const { return m_children & Height; }
    void clearElementHeight();

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    uint m_children;
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    enum Child {
        X = 1,
        Y = 2,
        Width = 4,
        Height = 8
    };

    DomRect(const DomRect &other);
    void operator = (const DomRect&other);
};

class QDESIGNER_UILIB_EXPORT DomLocale {
public:
    DomLocale();
    ~DomLocale();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeLanguage() const { return m_has_attr_language; }
    inline QString attributeLanguage() const { return m_attr_language; }
    inline void setAttributeLanguage(const QString& a) { m_attr_language = a; m_has_attr_language = true; }
    inline void clearAttributeLanguage() { m_has_attr_language = false; }

    inline bool hasAttributeCountry() const { return m_has_attr_country; }
    inline QString attributeCountry() const { return m_attr_country; }
    inline void setAttributeCountry(const QString& a) { m_attr_country = a; m_has_attr_country = true; }
    inline void clearAttributeCountry() { m_has_attr_country = false; }

    // child element accessors
private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_language;
    bool m_has_attr_language;

    QString m_attr_country;
    bool m_has_attr_country;

    // child element data

    DomLocale(const DomLocale &other);
    void operator = (const DomLocale&other);
};

class QDESIGNER_UILIB_EXPORT DomSizePolicy {
public:
    DomSizePolicy();
    ~DomSizePolicy();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeHSizeType() const { return m_has_attr_hSizeType; }
    inline QString attributeHSizeType() const { return m_attr_hSizeType; }
    inline void setAttributeHSizeType(const QString& a) { m_attr_hSizeType = a; m_has_attr_hSizeType = true; }
    inline void clearAttributeHSizeType() { m_has_attr_hSizeType = false; }

    inline bool hasAttributeVSizeType() const { return m_has_attr_vSizeType; }
    inline QString attributeVSizeType() const { return m_attr_vSizeType; }
    inline void setAttributeVSizeType(const QString& a) { m_attr_vSizeType = a; m_has_attr_vSizeType = true; }
    inline void clearAttributeVSizeType() { m_has_attr_vSizeType = false; }

    // child element accessors
    inline int elementHSizeType() const { return m_hSizeType; }
    void setElementHSizeType(int a);
    inline bool hasElementHSizeType() const { return m_children & HSizeType; }
    void clearElementHSizeType();

    inline int elementVSizeType() const { return m_vSizeType; }
    void setElementVSizeType(int a);
    inline bool hasElementVSizeType() const { return m_children & VSizeType; }
    void clearElementVSizeType();

    inline int elementHorStretch() const { return m_horStretch; }
    void setElementHorStretch(int a);
    inline bool hasElementHorStretch() const { return m_children & HorStretch; }
    void clearElementHorStretch();

    inline int elementVerStretch() const { return m_verStretch; }
    void setElementVerStretch(int a);
    inline bool hasElementVerStretch() const { return m_children & VerStretch; }
    void clearElementVerStretch();

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_hSizeType;
    bool m_has_attr_hSizeType;

    QString m_attr_vSizeType;
    bool m_has_attr_vSizeType;

    // child element data
    uint m_children;
    int m_hSizeType;
    int m_vSizeType;
    int m_horStretch;
    int m_verStretch;
    enum Child {
        HSizeType = 1,
        VSizeType = 2,
        HorStretch = 4,
        VerStretch = 8
    };

    DomSizePolicy(const DomSizePolicy &other);
    void operator = (const DomSizePolicy&other);
};

class QDESIGNER_UILIB_EXPORT DomSize {
public:
    DomSize();
    ~DomSize();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline int elementWidth() const { return m_width; }
    void setElementWidth(int a);
    inline bool hasElementWidth() const { return m_children & Width; }
    void clearElementWidth();

    inline int elementHeight() const { return m_height; }
    void setElementHeight(int a);
    inline bool hasElementHeight() const { return m_children & Height; }
    void clearElementHeight();

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    uint m_children;
    int m_width;
    int m_height;
    enum Child {
        Width = 1,
        Height = 2
    };

    DomSize(const DomSize &other);
    void operator = (const DomSize&other);
};

class QDESIGNER_UILIB_EXPORT DomDate {
public:
    DomDate();
    ~DomDate();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline int elementYear() const { return m_year; }
    void setElementYear(int a);
    inline bool hasElementYear() const { return m_children & Year; }
    void clearElementYear();

    inline int elementMonth() const { return m_month; }
    void setElementMonth(int a);
    inline bool hasElementMonth() const { return m_children & Month; }
    void clearElementMonth();

    inline int elementDay() const { return m_day; }
    void setElementDay(int a);
    inline bool hasElementDay() const { return m_children & Day; }
    void clearElementDay();

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    uint m_children;
    int m_year;
    int m_month;
    int m_day;
    enum Child {
        Year = 1,
        Month = 2,
        Day = 4
    };

    DomDate(const DomDate &other);
    void operator = (const DomDate&other);
};

class QDESIGNER_UILIB_EXPORT DomTime {
public:
    DomTime();
    ~DomTime();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline int elementHour() const { return m_hour; }
    void setElementHour(int a);
    inline bool hasElementHour() const { return m_children & Hour; }
    void clearElementHour();

    inline int elementMinute() const { return m_minute; }
    void setElementMinute(int a);
    inline bool hasElementMinute() const { return m_children & Minute; }
    void clearElementMinute();

    inline int elementSecond() const { return m_second; }
    void setElementSecond(int a);
    inline bool hasElementSecond() const { return m_children & Second; }
    void clearElementSecond();

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    uint m_children;
    int m_hour;
    int m_minute;
    int m_second;
    enum Child {
        Hour = 1,
        Minute = 2,
        Second = 4
    };

    DomTime(const DomTime &other);
    void operator = (const DomTime&other);
};

class QDESIGNER_UILIB_EXPORT DomDateTime {
public:
    DomDateTime();
    ~DomDateTime();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline int elementHour() const { return m_hour; }
    void setElementHour(int a);
    inline bool hasElementHour() const { return m_children & Hour; }
    void clearElementHour();

    inline int elementMinute() const { return m_minute; }
    void setElementMinute(int a);
    inline bool hasElementMinute() const { return m_children & Minute; }
    void clearElementMinute();

    inline int elementSecond() const { return m_second; }
    void setElementSecond(int a);
    inline bool hasElementSecond() const { return m_children & Second; }
    void clearElementSecond();

    inline int elementYear() const { return m_year; }
    void setElementYear(int a);
    inline bool hasElementYear() const { return m_children & Year; }
    void clearElementYear();

    inline int elementMonth() const { return m_month; }
    void setElementMonth(int a);
    inline bool hasElementMonth() const { return m_children & Month; }
    void clearElementMonth();

    inline int elementDay() const { return m_day; }
    void setElementDay(int a);
    inline bool hasElementDay() const { return m_children & Day; }
    void clearElementDay();

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    uint m_children;
    int m_hour;
    int m_minute;
    int m_second;
    int m_year;
    int m_month;
    int m_day;
    enum Child {
        Hour = 1,
        Minute = 2,
        Second = 4,
        Year = 8,
        Month = 16,
        Day = 32
    };

    DomDateTime(const DomDateTime &other);
    void operator = (const DomDateTime&other);
};

class QDESIGNER_UILIB_EXPORT DomStringList {
public:
    DomStringList();
    ~DomStringList();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline QStringList elementString() const { return m_string; }
    void setElementString(const QStringList& a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    QStringList m_string;

    DomStringList(const DomStringList &other);
    void operator = (const DomStringList&other);
};

class QDESIGNER_UILIB_EXPORT DomResourcePixmap {
public:
    DomResourcePixmap();
    ~DomResourcePixmap();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeResource() const { return m_has_attr_resource; }
    inline QString attributeResource() const { return m_attr_resource; }
    inline void setAttributeResource(const QString& a) { m_attr_resource = a; m_has_attr_resource = true; }
    inline void clearAttributeResource() { m_has_attr_resource = false; }

    inline bool hasAttributeAlias() const { return m_has_attr_alias; }
    inline QString attributeAlias() const { return m_attr_alias; }
    inline void setAttributeAlias(const QString& a) { m_attr_alias = a; m_has_attr_alias = true; }
    inline void clearAttributeAlias() { m_has_attr_alias = false; }

    // child element accessors
private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_resource;
    bool m_has_attr_resource;

    QString m_attr_alias;
    bool m_has_attr_alias;

    // child element data

    DomResourcePixmap(const DomResourcePixmap &other);
    void operator = (const DomResourcePixmap&other);
};

class QDESIGNER_UILIB_EXPORT DomString {
public:
    DomString();
    ~DomString();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeNotr() const { return m_has_attr_notr; }
    inline QString attributeNotr() const { return m_attr_notr; }
    inline void setAttributeNotr(const QString& a) { m_attr_notr = a; m_has_attr_notr = true; }
    inline void clearAttributeNotr() { m_has_attr_notr = false; }

    inline bool hasAttributeComment() const { return m_has_attr_comment; }
    inline QString attributeComment() const { return m_attr_comment; }
    inline void setAttributeComment(const QString& a) { m_attr_comment = a; m_has_attr_comment = true; }
    inline void clearAttributeComment() { m_has_attr_comment = false; }

    // child element accessors
private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_notr;
    bool m_has_attr_notr;

    QString m_attr_comment;
    bool m_has_attr_comment;

    // child element data

    DomString(const DomString &other);
    void operator = (const DomString&other);
};

class QDESIGNER_UILIB_EXPORT DomPointF {
public:
    DomPointF();
    ~DomPointF();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline double elementX() const { return m_x; }
    void setElementX(double a);
    inline bool hasElementX() const { return m_children & X; }
    void clearElementX();

    inline double elementY() const { return m_y; }
    void setElementY(double a);
    inline bool hasElementY() const { return m_children & Y; }
    void clearElementY();

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    uint m_children;
    double m_x;
    double m_y;
    enum Child {
        X = 1,
        Y = 2
    };

    DomPointF(const DomPointF &other);
    void operator = (const DomPointF&other);
};

class QDESIGNER_UILIB_EXPORT DomRectF {
public:
    DomRectF();
    ~DomRectF();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline double elementX() const { return m_x; }
    void setElementX(double a);
    inline bool hasElementX() const { return m_children & X; }
    void clearElementX();

    inline double elementY() const { return m_y; }
    void setElementY(double a);
    inline bool hasElementY() const { return m_children & Y; }
    void clearElementY();

    inline double elementWidth() const { return m_width; }
    void setElementWidth(double a);
    inline bool hasElementWidth() const { return m_children & Width; }
    void clearElementWidth();

    inline double elementHeight() const { return m_height; }
    void setElementHeight(double a);
    inline bool hasElementHeight() const { return m_children & Height; }
    void clearElementHeight();

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    uint m_children;
    double m_x;
    double m_y;
    double m_width;
    double m_height;
    enum Child {
        X = 1,
        Y = 2,
        Width = 4,
        Height = 8
    };

    DomRectF(const DomRectF &other);
    void operator = (const DomRectF&other);
};

class QDESIGNER_UILIB_EXPORT DomSizeF {
public:
    DomSizeF();
    ~DomSizeF();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline double elementWidth() const { return m_width; }
    void setElementWidth(double a);
    inline bool hasElementWidth() const { return m_children & Width; }
    void clearElementWidth();

    inline double elementHeight() const { return m_height; }
    void setElementHeight(double a);
    inline bool hasElementHeight() const { return m_children & Height; }
    void clearElementHeight();

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    uint m_children;
    double m_width;
    double m_height;
    enum Child {
        Width = 1,
        Height = 2
    };

    DomSizeF(const DomSizeF &other);
    void operator = (const DomSizeF&other);
};

class QDESIGNER_UILIB_EXPORT DomChar {
public:
    DomChar();
    ~DomChar();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline int elementUnicode() const { return m_unicode; }
    void setElementUnicode(int a);
    inline bool hasElementUnicode() const { return m_children & Unicode; }
    void clearElementUnicode();

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    uint m_children;
    int m_unicode;
    enum Child {
        Unicode = 1
    };

    DomChar(const DomChar &other);
    void operator = (const DomChar&other);
};

class QDESIGNER_UILIB_EXPORT DomUrl {
public:
    DomUrl();
    ~DomUrl();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline DomString* elementString() const { return m_string; }
    void setElementString(DomString* a);
    inline bool hasElementString() const { return m_children & String; }
    void clearElementString();

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    uint m_children;
    DomString* m_string;
    enum Child {
        String = 1
    };

    DomUrl(const DomUrl &other);
    void operator = (const DomUrl&other);
};

class QDESIGNER_UILIB_EXPORT DomProperty {
public:
    DomProperty();
    ~DomProperty();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeName() const { return m_has_attr_name; }
    inline QString attributeName() const { return m_attr_name; }
    inline void setAttributeName(const QString& a) { m_attr_name = a; m_has_attr_name = true; }
    inline void clearAttributeName() { m_has_attr_name = false; }

    inline bool hasAttributeStdset() const { return m_has_attr_stdset; }
    inline int attributeStdset() const { return m_attr_stdset; }
    inline void setAttributeStdset(int a) { m_attr_stdset = a; m_has_attr_stdset = true; }
    inline void clearAttributeStdset() { m_has_attr_stdset = false; }

    // child element accessors
    enum Kind { Unknown = 0, Bool, Color, Cstring, Cursor, CursorShape, Enum, Font, IconSet, Pixmap, Palette, Point, Rect, Set, Locale, SizePolicy, Size, String, StringList, Number, Float, Double, Date, Time, DateTime, PointF, RectF, SizeF, LongLong, Char, Url, UInt, ULongLong };
    inline Kind kind() const { return m_kind; }

    inline QString elementBool() const { return m_bool; }
    void setElementBool(const QString& a);

    inline DomColor* elementColor() const { return m_color; }
    void setElementColor(DomColor* a);

    inline QString elementCstring() const { return m_cstring; }
    void setElementCstring(const QString& a);

    inline int elementCursor() const { return m_cursor; }
    void setElementCursor(int a);

    inline QString elementCursorShape() const { return m_cursorShape; }
    void setElementCursorShape(const QString& a);

    inline QString elementEnum() const { return m_enum; }
    void setElementEnum(const QString& a);

    inline DomFont* elementFont() const { return m_font; }
    void setElementFont(DomFont* a);

    inline DomResourcePixmap* elementIconSet() const { return m_iconSet; }
    void setElementIconSet(DomResourcePixmap* a);

    inline DomResourcePixmap* elementPixmap() const { return m_pixmap; }
    void setElementPixmap(DomResourcePixmap* a);

    inline DomPalette* elementPalette() const { return m_palette; }
    void setElementPalette(DomPalette* a);

    inline DomPoint* elementPoint() const { return m_point; }
    void setElementPoint(DomPoint* a);

    inline DomRect* elementRect() const { return m_rect; }
    void setElementRect(DomRect* a);

    inline QString elementSet() const { return m_set; }
    void setElementSet(const QString& a);

    inline DomLocale* elementLocale() const { return m_locale; }
    void setElementLocale(DomLocale* a);

    inline DomSizePolicy* elementSizePolicy() const { return m_sizePolicy; }
    void setElementSizePolicy(DomSizePolicy* a);

    inline DomSize* elementSize() const { return m_size; }
    void setElementSize(DomSize* a);

    inline DomString* elementString() const { return m_string; }
    void setElementString(DomString* a);

    inline DomStringList* elementStringList() const { return m_stringList; }
    void setElementStringList(DomStringList* a);

    inline int elementNumber() const { return m_number; }
    void setElementNumber(int a);

    inline float elementFloat() const { return m_float; }
    void setElementFloat(float a);

    inline double elementDouble() const { return m_double; }
    void setElementDouble(double a);

    inline DomDate* elementDate() const { return m_date; }
    void setElementDate(DomDate* a);

    inline DomTime* elementTime() const { return m_time; }
    void setElementTime(DomTime* a);

    inline DomDateTime* elementDateTime() const { return m_dateTime; }
    void setElementDateTime(DomDateTime* a);

    inline DomPointF* elementPointF() const { return m_pointF; }
    void setElementPointF(DomPointF* a);

    inline DomRectF* elementRectF() const { return m_rectF; }
    void setElementRectF(DomRectF* a);

    inline DomSizeF* elementSizeF() const { return m_sizeF; }
    void setElementSizeF(DomSizeF* a);

    inline qlonglong elementLongLong() const { return m_longLong; }
    void setElementLongLong(qlonglong a);

    inline DomChar* elementChar() const { return m_char; }
    void setElementChar(DomChar* a);

    inline DomUrl* elementUrl() const { return m_url; }
    void setElementUrl(DomUrl* a);

    inline uint elementUInt() const { return m_uInt; }
    void setElementUInt(uint a);

    inline qulonglong elementULongLong() const { return m_uLongLong; }
    void setElementULongLong(qulonglong a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_name;
    bool m_has_attr_name;

    int m_attr_stdset;
    bool m_has_attr_stdset;

    // child element data
    Kind m_kind;
    QString m_bool;
    DomColor* m_color;
    QString m_cstring;
    int m_cursor;
    QString m_cursorShape;
    QString m_enum;
    DomFont* m_font;
    DomResourcePixmap* m_iconSet;
    DomResourcePixmap* m_pixmap;
    DomPalette* m_palette;
    DomPoint* m_point;
    DomRect* m_rect;
    QString m_set;
    DomLocale* m_locale;
    DomSizePolicy* m_sizePolicy;
    DomSize* m_size;
    DomString* m_string;
    DomStringList* m_stringList;
    int m_number;
    float m_float;
    double m_double;
    DomDate* m_date;
    DomTime* m_time;
    DomDateTime* m_dateTime;
    DomPointF* m_pointF;
    DomRectF* m_rectF;
    DomSizeF* m_sizeF;
    qlonglong m_longLong;
    DomChar* m_char;
    DomUrl* m_url;
    uint m_uInt;
    qulonglong m_uLongLong;

    DomProperty(const DomProperty &other);
    void operator = (const DomProperty&other);
};

class QDESIGNER_UILIB_EXPORT DomConnections {
public:
    DomConnections();
    ~DomConnections();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline QList<DomConnection*> elementConnection() const { return m_connection; }
    void setElementConnection(const QList<DomConnection*>& a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    QList<DomConnection*> m_connection;

    DomConnections(const DomConnections &other);
    void operator = (const DomConnections&other);
};

class QDESIGNER_UILIB_EXPORT DomConnection {
public:
    DomConnection();
    ~DomConnection();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline QString elementSender() const { return m_sender; }
    void setElementSender(const QString& a);
    inline bool hasElementSender() const { return m_children & Sender; }
    void clearElementSender();

    inline QString elementSignal() const { return m_signal; }
    void setElementSignal(const QString& a);
    inline bool hasElementSignal() const { return m_children & Signal; }
    void clearElementSignal();

    inline QString elementReceiver() const { return m_receiver; }
    void setElementReceiver(const QString& a);
    inline bool hasElementReceiver() const { return m_children & Receiver; }
    void clearElementReceiver();

    inline QString elementSlot() const { return m_slot; }
    void setElementSlot(const QString& a);
    inline bool hasElementSlot() const { return m_children & Slot; }
    void clearElementSlot();

    inline DomConnectionHints* elementHints() const { return m_hints; }
    void setElementHints(DomConnectionHints* a);
    inline bool hasElementHints() const { return m_children & Hints; }
    void clearElementHints();

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    uint m_children;
    QString m_sender;
    QString m_signal;
    QString m_receiver;
    QString m_slot;
    DomConnectionHints* m_hints;
    enum Child {
        Sender = 1,
        Signal = 2,
        Receiver = 4,
        Slot = 8,
        Hints = 16
    };

    DomConnection(const DomConnection &other);
    void operator = (const DomConnection&other);
};

class QDESIGNER_UILIB_EXPORT DomConnectionHints {
public:
    DomConnectionHints();
    ~DomConnectionHints();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline QList<DomConnectionHint*> elementHint() const { return m_hint; }
    void setElementHint(const QList<DomConnectionHint*>& a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    QList<DomConnectionHint*> m_hint;

    DomConnectionHints(const DomConnectionHints &other);
    void operator = (const DomConnectionHints&other);
};

class QDESIGNER_UILIB_EXPORT DomConnectionHint {
public:
    DomConnectionHint();
    ~DomConnectionHint();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeType() const { return m_has_attr_type; }
    inline QString attributeType() const { return m_attr_type; }
    inline void setAttributeType(const QString& a) { m_attr_type = a; m_has_attr_type = true; }
    inline void clearAttributeType() { m_has_attr_type = false; }

    // child element accessors
    inline int elementX() const { return m_x; }
    void setElementX(int a);
    inline bool hasElementX() const { return m_children & X; }
    void clearElementX();

    inline int elementY() const { return m_y; }
    void setElementY(int a);
    inline bool hasElementY() const { return m_children & Y; }
    void clearElementY();

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_type;
    bool m_has_attr_type;

    // child element data
    uint m_children;
    int m_x;
    int m_y;
    enum Child {
        X = 1,
        Y = 2
    };

    DomConnectionHint(const DomConnectionHint &other);
    void operator = (const DomConnectionHint&other);
};

class QDESIGNER_UILIB_EXPORT DomScript {
public:
    DomScript();
    ~DomScript();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    inline bool hasAttributeSource() const { return m_has_attr_source; }
    inline QString attributeSource() const { return m_attr_source; }
    inline void setAttributeSource(const QString& a) { m_attr_source = a; m_has_attr_source = true; }
    inline void clearAttributeSource() { m_has_attr_source = false; }

    inline bool hasAttributeLanguage() const { return m_has_attr_language; }
    inline QString attributeLanguage() const { return m_attr_language; }
    inline void setAttributeLanguage(const QString& a) { m_attr_language = a; m_has_attr_language = true; }
    inline void clearAttributeLanguage() { m_has_attr_language = false; }

    // child element accessors
private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    QString m_attr_source;
    bool m_has_attr_source;

    QString m_attr_language;
    bool m_has_attr_language;

    // child element data

    DomScript(const DomScript &other);
    void operator = (const DomScript&other);
};

class QDESIGNER_UILIB_EXPORT DomWidgetData {
public:
    DomWidgetData();
    ~DomWidgetData();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline QList<DomProperty*> elementProperty() const { return m_property; }
    void setElementProperty(const QList<DomProperty*>& a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    QList<DomProperty*> m_property;

    DomWidgetData(const DomWidgetData &other);
    void operator = (const DomWidgetData&other);
};

class QDESIGNER_UILIB_EXPORT DomDesignerData {
public:
    DomDesignerData();
    ~DomDesignerData();

    void read(const QDomElement &node);
    QDomElement write(QDomDocument &doc, const QString &tagName = QString()) const;
    inline QString text() const { return m_text; }
    inline void setText(const QString &s) { m_text = s; }

    // attribute accessors
    // child element accessors
    inline QList<DomProperty*> elementProperty() const { return m_property; }
    void setElementProperty(const QList<DomProperty*>& a);

private:
    QString m_text;
    void clear(bool clear_all = true);

    // attribute data
    // child element data
    QList<DomProperty*> m_property;

    DomDesignerData(const DomDesignerData &other);
    void operator = (const DomDesignerData&other);
};


#ifdef QFORMINTERNAL_NAMESPACE
}
#endif

#endif // UI4_H
