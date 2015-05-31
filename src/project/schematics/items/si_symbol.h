/*
 * EDA4U - Professional EDA for everyone!
 * Copyright (C) 2013 Urban Bruhin
 * http://eda4u.ubruhin.ch/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PROJECT_SI_SYMBOL_H
#define PROJECT_SI_SYMBOL_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/

#include <QtCore>
#include "si_base.h"
#include <eda4ucommon/fileio/if_xmlserializableobject.h>
#include <eda4ucommon/if_attributeprovider.h>
#include "../graphicsitems/sgi_symbol.h"

/*****************************************************************************************
 *  Forward Declarations
 ****************************************************************************************/

namespace project {
class Schematic;
class GenCompInstance;
class SI_SymbolPin;
}

namespace library {
class Symbol;
class GenCompSymbVarItem;
}

/*****************************************************************************************
 *  Class SI_Symbol
 ****************************************************************************************/

namespace project {

/**
 * @brief The SI_Symbol class
 *
 * @author ubruhin
 * @date 2014-08-23
 */
class SI_Symbol final : public SI_Base, public IF_XmlSerializableObject,
                        public IF_AttributeProvider
{
        Q_OBJECT

    public:

        // Constructors / Destructor
        explicit SI_Symbol(Schematic& schematic, const XmlDomElement& domElement) throw (Exception);
        explicit SI_Symbol(Schematic& schematic, GenCompInstance& genCompInstance,
                           const QUuid& symbolItem, const Point& position = Point(),
                           const Angle& angle = Angle()) throw (Exception);
        ~SI_Symbol() noexcept;

        // Getters
        Schematic& getSchematic() const noexcept {return mSchematic;}
        const QUuid& getUuid() const noexcept {return mUuid;}
        const Angle& getAngle() const noexcept {return mAngle;}
        QString getName() const noexcept;
        SI_SymbolPin* getPin(const QUuid& pinUuid) const noexcept {return mPins.value(pinUuid);}
        const QHash<QUuid, SI_SymbolPin*>& getPins() const noexcept {return mPins;}
        GenCompInstance& getGenCompInstance() const noexcept {return *mGenCompInstance;}
        const library::Symbol& getLibSymbol() const noexcept {return *mSymbol;}
        const library::GenCompSymbVarItem& getGenCompSymbVarItem() const noexcept {return *mSymbVarItem;}

        // Setters
        void setPosition(const Point& newPos) throw (Exception);
        void setAngle(const Angle& newAngle) throw (Exception);

        // General Methods
        void addToSchematic(GraphicsScene& scene) throw (Exception);
        void removeFromSchematic(GraphicsScene& scene) throw (Exception);
        XmlDomElement* serializeToXmlDomElement() const throw (Exception);

        // Helper Methods
        Point mapToScene(const Point& relativePos) const noexcept;
        bool getAttributeValue(const QString& attrNS, const QString& attrKey,
                               bool passToParents, QString& value) const noexcept;

        // Inherited from SI_Base
        Type_t getType() const noexcept override {return SI_Base::Type_t::Symbol;}
        const Point& getPosition() const noexcept override {return mPosition;}
        QPainterPath getGrabAreaScenePx() const noexcept override;
        void setSelected(bool selected) noexcept override;


    private slots:

        void schematicOrGenCompAttributesChanged();


    signals:

        /// @copydoc IF_AttributeProvider#attributesChanged()
        void attributesChanged();


    private:

        // make some methods inaccessible...
        SI_Symbol();
        SI_Symbol(const SI_Symbol& other);
        SI_Symbol& operator=(const SI_Symbol& rhs);

        // Private Methods
        void init(const QUuid& symbVarItemUuid) throw (Exception);
        bool checkAttributesValidity() const noexcept;


        // General
        Schematic& mSchematic;
        GenCompInstance* mGenCompInstance;
        const library::GenCompSymbVarItem* mSymbVarItem;
        const library::Symbol* mSymbol;
        QHash<QUuid, SI_SymbolPin*> mPins; ///< key: symbol pin UUID
        SGI_Symbol* mGraphicsItem;

        // Attributes
        QUuid mUuid;
        Point mPosition;
        Angle mAngle;
};

} // namespace project

#endif // PROJECT_SI_SYMBOL_H
