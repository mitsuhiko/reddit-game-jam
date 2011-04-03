#ifndef _INC_PD_XML_HPP_
#define _INC_PD_XML_HPP_
#include <pd/pd.hpp>
#include <pd/lexical_cast.hpp>
#include <iterator>

#define TIXML_USE_STL
#include <tinyxml/tinyxml.h>

namespace pd {

    class xml_attribute {
    public:
        xml_attribute(TiXmlAttribute *attr) : m_attr(attr)
        {
        }

        xml_attribute next() const
        {
            return xml_attribute(m_attr ? m_attr->Next() : 0);
        }

        xml_attribute previous() const
        {
            assert(m_attr);
            return xml_attribute(m_attr ? m_attr->Previous() : 0);
        }

        const std::string &key() const
        {
            static std::string dummy_key = "";
            return m_attr ? m_attr->NameTStr() : dummy_key;
        }

        const std::string &value(const std::string &default_val = "") const
        {
            return m_attr ? m_attr->ValueStr() : default_val;
        }

        template <class T>
        T value(const T &default_val = T()) const
        {
            if (!m_attr)
                return default_val;
            bool okay;
            T rv = pd::lexical_cast<T>(m_attr->ValueStr(), okay);
            return okay ? rv : default_val;
        }

        operator bool() const
        {
            return m_attr != 0;
        }

        bool operator!() const
        {
            return !static_cast<bool>(*this);
        }

    private:
        mutable TiXmlAttribute *m_attr;
    };

    class xml_element {
    public:
        xml_element(TiXmlElement *elm) : m_elm(elm)
        {
        }

        xml_element first_child()
        {
            TiXmlNode *node = m_elm->FirstChild();
            while (node && node->ToElement() == 0)
                node = node->NextSibling();
            return xml_element(node ? node->ToElement() : 0);
        }

        xml_element first_child(const std::string &tag)
        {
            xml_element node = first_child();
            if (node.name() != tag)
                node = node.next_sibling(tag);
            return node;
        }

        xml_element last_child()
        {
            TiXmlNode *node = m_elm->LastChild();
            while (node && node->ToElement() == 0)
                node = node->PreviousSibling();
            return xml_element(node ? node->ToElement() : 0);
        }

        xml_element last_child(const std::string &tag)
        {
            xml_element node = last_child();
            if (node.name() != tag)
                node = node.previous_sibling(tag);
            return node;
        }

        xml_element next_sibling()
        {
            TiXmlNode *node = m_elm->NextSibling();
            while (node && node->ToElement() == 0)
                node = node->NextSibling();
            return xml_element(node ? node->ToElement() : 0);
        }

        xml_element next_sibling(const std::string &name)
        {
            TiXmlNode *node = m_elm->NextSibling();
            while (node && (node->ToElement() == 0 ||
                            node->ToElement()->ValueStr() != name))
                node = node->NextSibling();
            return xml_element(node ? node->ToElement() : 0);
        }

        xml_element previous_sibling()
        {
            TiXmlNode *node = m_elm->PreviousSibling();
            while (node && node->ToElement() == 0)
                node = node->PreviousSibling();
            return xml_element(node ? node->ToElement() : 0);
        }

        xml_element previous_sibling(const std::string &name)
        {
            TiXmlNode *node = m_elm->PreviousSibling();
            while (node && (node->ToElement() == 0 ||
                            node->ToElement()->ValueStr() != name))
                node = node->PreviousSibling();
            return xml_element(node ? node->ToElement() : 0);
        }

        xml_attribute first_attribute()
        {
            return xml_attribute(m_elm ? m_elm->FirstAttribute() : 0);
        }

        xml_attribute last_attribute()
        {
            return xml_attribute(m_elm ? m_elm->LastAttribute() : 0);
        }

        operator bool() const
        {
            return m_elm != 0;
        }

        bool operator!() const
        {
            return !static_cast<bool>(*this);
        }

        bool operator==(const pd::xml_element &other) const
        {
            return m_elm == other.m_elm;
        }

        bool operator!=(const pd::xml_element &other) const
        {
            return !(*this == other);
        }

        const std::string &name() const
        {
            static std::string dummy_name = "?";
            return m_elm ? m_elm->ValueStr() : dummy_name;
        }

        const std::string &text(const std::string &default_val = "") const
        {
            const TiXmlNode *child = m_elm ? m_elm->FirstChild() : 0;
            if (!child)
                return default_val;
            const TiXmlText *text = child->ToText();
            return text ? text->ValueStr() : default_val;
        }

        const std::string &tail(const std::string &default_val = "") const
        {
            const TiXmlNode *next = m_elm ? m_elm->NextSibling() : 0;
            if (!next)
                return default_val;
            const TiXmlText *text = next ? next->ToText() : 0;
            return text ? text->ValueStr() : default_val;
        }

        const bool has_attr(const std::string &name) const
        {
            return m_elm && m_elm->Attribute(name) != 0;
        }

        const std::string &attr(const std::string &name,
                                const std::string &default_val = "") const
        {
            if (!m_elm)
                return default_val;
            const std::string *rv = m_elm->Attribute(name);
            return rv ? *rv : default_val;
        }

        template <class T>
        T attr(const std::string &name, const T &default_val = T()) const
        {
            if (!m_elm)
                return default_val;
            const std::string *val = m_elm->Attribute(name);
            if (!val)
                return default_val;
            bool okay;
            T rv = pd::lexical_cast<T>(*val, okay);
            return okay ? rv : default_val;
        }

    private:
        mutable TiXmlElement *m_elm;
    };

    class xml_document {
    public:
        xml_document()
        {
            m_doc.SetCondenseWhiteSpace(false);
        }

        xml_document(const std::string &filename)
        {
            if (!load(filename))
                pd::critical_error("XML Load error", "Could not load XML file");
        }

        bool load(const std::string &filename)
        {
            return m_doc.LoadFile(filename, TIXML_ENCODING_UTF8);
        }

        xml_element root() const
        {
            return xml_element(m_doc.RootElement());
        }

    private:
        mutable TiXmlDocument m_doc;
    };
}

#endif
