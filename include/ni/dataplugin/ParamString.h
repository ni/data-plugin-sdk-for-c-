#pragma once

#include <string>
#include <map>

namespace ni {
  namespace dataplugin {
    namespace str {

      /**
      \brief  Class is used to divorce a string (std::string or const char*) into pieces.
              It is possible to give the Separators.
              e.g. "*.dat ; *.atf " -> "*.dat","*.atf"
      */
      template<class StrType> class Token
      {
      public:
        /**
        \brief Constructor.
        \param tokenstring  [IN]  String that should be divided into tokens.
        */
        Token(const StrType& tokenstring) :
          m_tokenString(tokenstring)
        {
          m_pos1 = m_pos2 = 0;
        };

        /**
        \brief Constructor.
        \param tokenstring  [IN]  String that should be divided into tokens.
        \param separators   [IN]  Token separators (e.g. " /t/n,;" )
        */
        Token(const typename StrType::value_type* tokenstring) :
          m_tokenString(tokenstring)
        {
          m_pos1 = m_pos2 = 0;
        };

        /**
        \brief  Get the next Token.
        \param  separators [IN] Can contain a list of separators that will be used
                                instead of the separators given with the constructor
                                of the class. (Works like the c std strtok function.)
        \return NULL if no following token could be found.
        */
        const StrType* Next(const typename StrType::value_type* separators)
        {
          return this->Next(separators, separators);
        };

        /**
        \brief  Get the next Token.
        \param  separators [IN] Can contain a list of separators that will be used
                                instead of the separators given with the constructor
                                of the class. (Works like the c std strtok function.)
        \return NULL if no following token could be found.
        */
        const StrType* Next(const typename StrType::value_type separator)
        {
          return this->Next(separator, separator);
        };

        /**
        \brief  Get the next Token.
        \param  startSeparators [IN]  Can contain a list of separators that will be used
                                      instead of the separators given with the constructor
                                      of the class. (Works like the c std strtok function.)
        \param  stopSeparators [IN] Can contain a list of separators that will be used
                                    instead of the separators given with the constructor
                                    of the class. (Works like the c std strtok function.)
        \return NULL if no following token could be found.
        */
        const StrType* Next(const typename StrType::value_type* startSeparators, const typename StrType::value_type* stopSeparators)
        {
          StrType* rc = 0;
          bool seperatorFound;

          m_pos1 = m_pos2;

          m_pos1 = m_tokenString.find_first_not_of(startSeparators, m_pos1);

          if (StrType::npos != m_pos1)
          {
            m_pos2 = m_pos1;

            m_pos2 = m_tokenString.find_first_of(stopSeparators, m_pos2);
            if (StrType::npos == m_pos2)
            {
              m_pos2 = m_tokenString.size();
              seperatorFound = false;
            }
            else
            {
              seperatorFound = true;
            }

            if (m_pos1 != m_pos2)
            {
              m_actualToken = m_tokenString.substr(m_pos1, m_pos2 - m_pos1);
              rc = &m_actualToken;

              if (seperatorFound && (m_pos2 != m_tokenString.size()))
              {
                m_pos2 += 1;
              }
            }
            else
            {
              bool seperatorsEqual(startSeparators == stopSeparators ? true : (0 == StrType(startSeparators).compare(stopSeparators)));
              if (!seperatorsEqual)
              {
                if (m_pos2 < m_tokenString.size())
                {
                  m_actualToken.resize(0);
                  rc = &m_actualToken;

                  if (seperatorFound && (m_pos2 != m_tokenString.size()))
                  {
                    m_pos2 += 1;
                  }
                }
              }
            }
          }

          return rc;
        };

        /**
        \brief  Get the next Token.
        \param  startSeparators [IN]  Can contain a list of separators that will be used
                                      instead of the separators given with the constructor
                                      of the class. (Works like the c std strtok function.)
        \param  stopSeparators [IN] Can contain a list of separators that will be used
                                    instead of the separators given with the constructor
                                    of the class. (Works like the c std strtok function.)
        \return NULL if no following token could be found.
        */
        const StrType* Next(const typename StrType::value_type startSeparator, const typename StrType::value_type stopSeparator)
        {
          StrType* rc = 0;
          bool seperatorFound;

          m_pos1 = m_pos2;

          m_pos1 = m_tokenString.find_first_not_of(StrType(&startSeparator, 1), m_pos1);

          if (StrType::npos != m_pos1)
          {
            m_pos2 = m_pos1;

            m_pos2 = m_tokenString.find(stopSeparator, m_pos2);
            if (StrType::npos == m_pos2)
            {
              m_pos2 = m_tokenString.size();
              seperatorFound = false;
            }
            else
            {
              seperatorFound = true;
            }

            if (m_pos1 != m_pos2)
            {
              m_actualToken = m_tokenString.substr(m_pos1, m_pos2 - m_pos1);
              rc = &m_actualToken;

              if (seperatorFound && (m_pos2 != m_tokenString.size()))
              {
                m_pos2 += 1;
              }
            }
            else
            {
              if (startSeparator != stopSeparator)
              {
                if (m_pos2 < m_tokenString.size())
                {
                  m_actualToken.resize(0);
                  rc = &m_actualToken;

                  if (seperatorFound && (m_pos2 != m_tokenString.size()))
                  {
                    m_pos2 += 1;
                  }
                }
              }
            }
          }

          return rc;
        };

      private:
        /** Default constructor forbidden. */
        Token() {};
        /** assignment operator forbidden. */
        Token& operator=(const Token&);
        /** \brief  Represents the actual selected token. */
        StrType             m_actualToken;
        /** \brief  The original Token String is stored here. */
        const StrType       m_tokenString;
        /** \brief  Start position of the actual token. */
        typename StrType::size_type  m_pos1;
        /** \brief  End position of the actual token. */
        typename StrType::size_type  m_pos2;
      };

      /**
      \brief  Remove leading spaces, tabs etc.
              trim string. e.g. "  \t Hallo  \r\n" -> "Hallo  \r\n"
      \param  s           [IN]  string to be trimmed.
      \param  delimiters  [IN]  delimters that should be removed
      */
      template<class T> inline void TrimLeftInplace(T& s, const wchar_t* delimiters = L" \n\r\t")
      {
        // trim left
        s.erase(0, s.find_first_not_of(delimiters));
      }

      /**
      \brief  Remove attached spaces, tabs etc.
              trimed string. e.g. "  \t Hallo  \r\n" -> "  \t Hallo"
      \param  s           [IN]  string to be trimmed.
      \param  delimiters  [IN]  delimters that should be removed
      */
      template<class T> inline void TrimRightInplace(T& s, const wchar_t* delimiters = L" \n\r\t")
      {
        // trim right
        s.erase(s.find_last_not_of(delimiters) + 1);
      }

      /**
      \brief  Remove leading and attached spaces, tabs etc.
      trimed string. e.g. "  \t Hallo  \r\n" -> "Hallo"
      \param  s           [IN]  string to be trimmed.
      \param  delimiters  [IN]  delimters that should be removed
      \see    TrimLeftInplace TrimRightInplace
      */
      template<class T> inline void TrimInplace(T& s, const wchar_t* delimiters = L" \n\r\t")
      {
        TrimLeftInplace(s, delimiters);
        TrimRightInplace(s, delimiters);
      }

      /**
      \brief  Remove leading spaces, tabs etc.
      \param  s           [IN]  string to be trimmed.
      \param  delimiters  [IN]  delimters that should be removed
      \return trimed string. e.g. "  \t Hallo  \r\n" -> "Hallo  \r\n"
      */
      template<class T> inline T TrimLeft(const T& s, const wchar_t* delimiters = L" \n\r\t")
      {
        T rv(s);
        TrimLeftInplace(rv, delimiters);
        return rv;
      }

      /**
      \brief  Remove attached spaces, tabs etc.
      \param  s           [IN]  string to be trimmed.
      \param  delimiters  [IN]  delimters that should be removed
      \return trimed string. e.g. "  \t Hallo  \r\n" -> "  \t Hallo"
      */
      template<class T> inline T TrimRight(const T& s, const wchar_t* delimiters = L" \n\r\t")
      {
        T rv(s);
        TrimRightInplace(rv, delimiters);
        return rv;
      }

      /**
      \brief  Remove leading and attached spaces, tabs etc.
      \return trimed string. e.g. "  \t Hallo  \r\n" -> "Hallo"
      \param  s           [IN]  string to be trimmed.
      \param  delimiters  [IN]  delimters that should be removed
      \see    TrimLeft TrimRight
      */
      template<class T> inline T Trim(const T& s, const wchar_t* delimiters = L" \n\r\t")
      {
        T rv(s);
        TrimInplace(rv, delimiters);
        return rv;
      }

      class Param {
      public:

        Param(const wchar_t* parameter_string) :
          m_paramString(parameter_string),
          m_currPos(0),
          m_tokenStartPos(0),
          m_tokenLength(0),
          m_valueStartPos(0),
          m_valueLength(0)
        {
        }

        Param(const std::wstring& parameter_string) :
          m_paramString(parameter_string),
          m_currPos(0),
          m_tokenStartPos(0),
          m_tokenLength(0),
          m_valueStartPos(0),
          m_valueLength(0)
        {
        }


        bool Next()
        {
          bool rc(true); // return code
          std::wstring::size_type strFindPos(m_paramString.find(L'<', m_currPos));
          if (std::wstring::npos == strFindPos) {
            rc = false;
          }
          else {
            m_tokenStartPos = strFindPos + 1;

            strFindPos = m_paramString.find(L'>', m_currPos);
            if (std::wstring::npos == strFindPos) {
              rc = false;
            }
            else {
              m_tokenLength = strFindPos - m_tokenStartPos;

              // token identified
              std::wstring endtoken = L"</" + Token() + L">";

              strFindPos = m_paramString.find(endtoken, m_tokenStartPos + m_tokenLength + 1);
              if (std::wstring::npos == strFindPos) {
                rc = false;
              }
              else {
                m_valueStartPos = m_tokenStartPos + m_tokenLength + 1; // , strFindPos - 1 - (m_tokenStartPos + m_tokenLength + 2));
                m_valueLength = ((strFindPos - 1) + 1) - m_valueStartPos;  //, m_tokenStartPos + m_tokenLength + 1) + 1 ) - m_valueStartPos;
                m_currPos = strFindPos + endtoken.size();
              }
            }
          }

          return(rc);
        }

        std::wstring Token() const
        {
          std::wstring rc;// return code
          std::wstring::size_type strEndPos(m_paramString.find_first_of(L" >\t\n\r", m_tokenStartPos));

          if (std::wstring::npos == strEndPos) {
            rc = L"";
          }
          else {
            rc = m_paramString.substr(m_tokenStartPos, strEndPos - m_tokenStartPos);
          }

          return(rc);
        }

        std::wstring Attributes() const
        {
          std::wstring rc;// return code
          std::wstring::size_type strStartPos = m_paramString.find_first_of(L" \t>", m_tokenStartPos) + 1;
          std::wstring::size_type strLength = m_tokenStartPos + m_tokenLength - strStartPos;

          if (std::wstring::npos == strLength)
          {
            rc = L"";
          }
          else
          {
            rc = m_paramString.substr(strStartPos, strLength);
          }

          return(rc);
        }

        typedef std::map<std::wstring, std::wstring> AttributeMap;

        AttributeMap GetAttributeMap() const
        {
          AttributeMap aMap;

          std::wstring attribs = Attributes();

          aMap = this->GetAttributeMap(attribs);

          return aMap;
        }

        static AttributeMap GetAttributeMap(const std::wstring& attributes)
        {
          AttributeMap aMap;

          if (attributes.empty())
            return aMap;

          ni::dataplugin::str::Token<std::wstring> tokenizer(attributes);

          const std::wstring *token;
          while (NULL != (token = tokenizer.Next(L" \t", L"=\r\n\0")))
          {
            const std::wstring key(*token);

            token = tokenizer.Next(L" =\t\n\r\0", L" \t\n\r\0");
            if (token)
            {
              std::wstring attribute = *token;

              if (L'\"' == attribute[0])
              {
                if ((attribute.size() == 1) ||
                  (L'\"' != attribute[attribute.size() - 1]))
                {
                  const std::wstring* attribValue = tokenizer.Next(L"", L"\"");
                  if (NULL != attribValue) {

                    attribute += L" ";
                    attribute += attribValue->c_str();
                  }
                }
              }
              else if (L'\'' == attribute[0])
              {
                if ((attribute.size() == 1) ||
                  (L'\'' != attribute[attribute.size() - 1]))
                {
                  const std::wstring* attribValue = tokenizer.Next(L"", L"'");
                  if (NULL != attribValue) {

                    attribute += L" ";
                    attribute += attribValue->c_str();
                  }
                }
              }

              attribute = ni::dataplugin::str::Trim(attribute);
              aMap[ni::dataplugin::str::Trim(key)] = ni::dataplugin::str::Trim(attribute, L"\"'");
            }
            else
            {
              aMap[ni::dataplugin::str::Trim(key)] = L"";
            }

          }

          return aMap;
        }

        std::wstring Value() const
        {
          std::wstring rc;// return code

          rc = m_paramString.substr(m_valueStartPos, m_valueLength);

          return(rc);
        }

        void Reset(void)
        {
          m_currPos = 0;
          m_tokenStartPos = 0;
          m_tokenLength = 0;
          m_valueStartPos = 0;
          m_valueLength = 0;
        }

        void Reset(const std::wstring& parameter_string)
        {
          m_paramString = parameter_string;
          m_currPos = 0;
          m_tokenStartPos = 0;
          m_tokenLength = 0;
          m_valueStartPos = 0;
          m_valueLength = 0;
        }

        bool DoesTagExist(const std::wstring& tag) const
        {
          bool rc = false; // return code

          Param tokparam(m_paramString);
          while (tokparam.Next())
          {
            if (tokparam.Token() == tag)
            {
              rc = true;
              break;
            }
          }

          return rc;
        }

        bool FindTag(const std::wstring& tag, std::wstring& value, std::wstring& attributes) const
        {
          bool rc = false; // return code
          Param tokparam(m_paramString);

          while (tokparam.Next())
          {
            if (0 == tokparam.Token().compare(tag))
            {
              value = tokparam.Value();
              attributes = tokparam.Attributes();
              rc = true;
              break;
            }
          }

          return rc;
        }

        bool FindTag(const std::wstring& tag, std::wstring& value, AttributeMap& attributeMap) const
        {
          bool rc = false; // return code
          Param tokparam(m_paramString);

          while (tokparam.Next())
          {
            if (0 == tokparam.Token().compare(tag))
            {
              value = tokparam.Value();
              attributeMap = tokparam.GetAttributeMap();
              rc = true;
              break;
            }
          }

          return rc;
        }

        std::wstring GetTagValue(const std::wstring& tag) const
        {
          std::wstring value, attribute;

          if (FindTag(tag, value, attribute))
          {
            return value;

          }
          return std::wstring();
        }


        AttributeMap GetTagAttributeMap(const std::wstring& tag) const
        {
          std::wstring   value;
          AttributeMap  attributeMap;

          FindTag(tag, value, attributeMap);

          return attributeMap;
        }


        std::wstring GetStringAfterActualToken() const
        {
          std::wstring rv;

          rv = m_paramString.substr(m_currPos);

          return rv;
        }

        bool TagExists(const std::wstring& tag) const
        {
          bool rc; // return code
          std::wstring value, attribute;

          if (FindTag(tag, value, attribute))
          {
            rc = true;
          }
          else
          {
            rc = false;
          }

          return rc;
        }

      private:
        std::wstring m_paramString;    /**< Contains the string to be chopped */
        std::wstring::size_type  m_currPos;        /**< Stores the current start position */

        std::wstring::size_type m_tokenStartPos;  /**< Stores the Token (Tag) start position */
        std::wstring::size_type m_tokenLength;    /**< Stores the token length */
        std::wstring::size_type m_valueStartPos;  /**< Stores the Value start Position */
        std::wstring::size_type m_valueLength;    /**< Stores the Value length*/
      };

      /**
         \brief  Used to simply replace a string in a string

         Example :
         \code
         std::wstring str(L"ab ab");

         ReplaceInplace<std::wstring>(str, L"ab", L"cdef");
         assert(0 == str.compare(L"cdef cdef"));
         \endcode

         \param  str             Original String that contains the elements that should be replaced
         \param  searchString    String element to be replaced by replaceString
         \param  replaceString   Replace string element
      */
      template<class T> inline void ReplaceInplace(
        T& str,
        const T& searchString,
        const T& replaceString)
      {
        const T::size_type searchStringSize(searchString.size());
        if (0 == searchStringSize) {
          return;
        }
        const T::size_type replaceStringSize(replaceString.size());

        T::size_type n = 0;
        while (T::npos != (n = str.find(searchString, n))) {
          str.replace(n, searchStringSize, replaceString);
          n += replaceStringSize;
        }
      }

      template<typename T> T Unescape(const T& src)
      {
        T target(src);
        ni::dataplugin::str::ReplaceInplace<T>(target, L"&lt;", L"<");
        ni::dataplugin::str::ReplaceInplace<T>(target, L"&gt;", L">");
        ni::dataplugin::str::ReplaceInplace<T>(target, L"&apos;", L"\'");
        ni::dataplugin::str::ReplaceInplace<T>(target, L"&quot;", L"\"");
        ni::dataplugin::str::ReplaceInplace<T>(target, L"&amp;", L"&");
        return target;
      }

    }
  }
}
