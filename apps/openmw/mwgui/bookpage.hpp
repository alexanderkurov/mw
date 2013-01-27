#ifndef MWGUI_BOOKPAGE_HPP
#define MWGUI_BOOKPAGE_HPP

#include "MyGUI_Colour.h"
#include "MyGUI_Widget.h"

#include <functional>
#include <platform/stdint.h>

namespace MWGui
{
    /// A formatted and paginated document to be used with
    /// the book page widget.
    struct ITypesetBook
    {
        typedef std::shared_ptr <ITypesetBook> ptr;
        typedef intptr_t interactive_id;

        /// Returns the number of pages in the document.
        virtual int pageCount () const = 0;

        /// Return the area covered by the document. The first
        /// integer is the maximum with of any line. This is not
        /// the largest coordinate of the right edge of any line,
        /// it is the largest distance from the left edge to the
        /// right edge. The second integer is the height of all
        /// text combined prior to pagination.
        virtual std::pair <int, int> getSize () const = 0;
    };

    /// A factory class for creating a typeset book instance.
    struct IBookTypesetter
    {
        typedef std::shared_ptr <IBookTypesetter> ptr;
        typedef ITypesetBook::interactive_id interactive_id;
        typedef MyGUI::Colour coulour;
        typedef uint8_t const * utf8_point;
        typedef std::pair <utf8_point, utf8_point> utf8_span;

        enum alignment {
            alignLeft   = -1,
            alignCenter = 0,
            alignRight  = +1
        };

        /// Styles are used to control the character level formatting
        /// of text added to a typeset book. Their lifetime is equal
        /// to the lifetime of the book-typesetter instance that created
        /// them.
        struct IStyle;

        /// A factory function for creating the default implementation of a book typesetter
        static ptr create (int pageWidth, int pageHeight);

        /// Create a simple text style consisting of a font and a text color.
        virtual IStyle* createStyle (char const * Font, coulour Colour) = 0;

        /// Create a hyper-link style with a user-defined identifier based on an
        /// existing style. The unique flag forces a new instance of this style
        /// to be created even if an existing instance is present.
        virtual IStyle* createHotStyle (IStyle * BaseStyle, coulour NormalColour, coulour HoverColour, coulour ActiveColour, interactive_id Id, bool Unique = true) = 0;

        /// Insert a line break into the document. Newline characters in the input
        /// text have the same affect. The margin parameter adds additional space
        /// before the next line of text.
        virtual void lineBreak (float margin = 0) = 0;

        /// Insert a section  break into the document. This causes a new section
        /// to begin when additional text is inserted. Pagination attempts to keep
        /// sections together on a single page. The margin parameter adds additional space
        /// before the next line of text.
        virtual void sectionBreak (float margin = 0) = 0;

        /// Changes the alignment for the current section of text.
        virtual void setSectionAlignment (alignment sectionAlignment) = 0;

        // Layout a block of text with the specified style into the document.
        virtual void write (IStyle * Style, utf8_span Text) = 0;

        /// Adds a content block to the document without laying it out. An
        /// identifier is returned that can be used to refer to it. If select
        /// is true, the block is activated to be references by future writes.
        virtual intptr_t add_content (utf8_span Text, bool Select = true) = 0;

        /// Select a previously created content block for future writes.
        virtual void select_content (intptr_t contentHandle) = 0;

        /// Layout a span of the selected content block into the document
        /// using the specified style.
        virtual void write (IStyle * Style, size_t Begin, size_t End) = 0;

        /// Finalize the document layout, and return a pointer to it.
        virtual ITypesetBook::ptr complete () = 0;
    };

    /// An interface to the BookPage widget.
    class IBookPage : public MyGUI::Widget
    {
    MYGUI_RTTI_DERIVED(IBookPage)
    public:

        typedef ITypesetBook::interactive_id interactive_id;
        typedef std::function <void (interactive_id)> click_callback;

        /// Make the widget display the specified page from the specified book.
        virtual void showPage (ITypesetBook::ptr Book, size_t Page) = 0;

        /// Set the callback for a clicking a hyper-link in the document.
        virtual void adviseLinkClicked (click_callback callback) = 0;

        /// Clear the hyper-link click callback.
        virtual void unadviseLinkClicked () = 0;

        /// Register the widget and associated sub-widget with MyGUI. Should be
        /// called once near the beginning of the program.
        static void registerMyGUIComponents ();
    };
}

#endif // MWGUI_BOOKPAGE_HPP
