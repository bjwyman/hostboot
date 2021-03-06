/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/usr/pore/poreve/model/poreregister.H $                    */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* COPYRIGHT International Business Machines Corp. 2012,2014              */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
#ifndef __VSBE_POREREGISTER_H
#define __VSBE_POREREGISTER_H

// $Id: poreregister.H,v 1.3 2012/06/15 12:34:39 bcbrock Exp $

/// \file poreregister.H
/// \brief The PoreRegister and PoreRegisterWritable classes
///
/// This header also includes selected register layout definitions

#include <stdint.h>

#include "poreconstants.H"

namespace vsbe {

    class PoreRegister;
    class PoreRegisterWritable;
    class PoreDataBuffer;
    class PoreInterface;
};


////////////////////////////////////////////////////////////////////////////
// PoreRegister
////////////////////////////////////////////////////////////////////////////

/// An abstract PORE programmer-visible register model
///
/// This class is introduced to provide abstract register models that are easy
/// to program for user code (e.g., hooks), by making the register appear to
/// be a data member of the PoreInterface, but leaving the implementation to
/// the underlying PoreModel.  This is necessary in part because there is not
/// always a direct map between the PORE hardware register and the
/// programmer-visible register (or abstract register like CIA).
///
/// Two types of register are supported here:
///
/// - True programmer-visible registers like D0, A0, P0, PC
/// - "Abstract" registers introduced to simplify hook programming like CIA
///
/// The register values manipulated by the PoreRegister associated with true
/// programmer visible registers are always the programmer-visible register
/// contents.  Reading the register always returns the value as if the
/// register were moved to a 64-bit data register inside the engine.  Writing
/// the register updates the underlying hardware model as if the register were
/// updated by a move from a 64-bit data register.
///
/// The PoreRegister defines a conversion operator to a uint64_t.  This allows
/// the register object to be used as if it were a uint64_t data member of the
/// PoreInterface. Note that for registers smaller than 64 bits the data will
/// be (for reads), or is expected to be (for writes), right justfied in the
/// 64 bit input or result.
///
/// When the PoreRegister instances are defined as data members of
/// PoreInterface, each instance is parameterized with the information
/// necessary to transform an access of the abstract register into the
/// register interface calls of the PoreModel. Note that in certain cases it
/// may not be possible to use the abstract register directly in an expression
/// because the usage is ambiguous to the compiler.  These problems can
/// be solved either by using temporary variables to disambiguate the usage,
/// or by explicitly using the read() methods of the PoreRegister
/// objects. 
///
/// All registers have read access and read() methods.  Most are actually
/// implemeted as the PoreRegisterWritable subclass that has write access and
/// a write method as well.

class 
vsbe::PoreRegister {

public:

    ////////////////////////////// Creators //////////////////////////////

    /// Create a PoreRegister
    ///
    /// \param[in] i_interface A pointer to the parent PoreInterface object that
    /// implements the underlying registerRead() and registerWrite() methods.
    ///
    /// \param[in] i_register A value from the enumeration
    /// PoreRegisterEncodingore identifying the register.
    PoreRegister(PoreInterface* i_interface, 
                 const PoreRegisterEncoding i_register);

    virtual ~PoreRegister();

    ///////////////////////////// Accessors //////////////////////////////

    /// Implement the PoreRegister register read
    ///
    /// Note that if the embedded call of PoreInterface::registerRead() fails
    /// it is considered a bug in the model.
    ///
    /// \retval value The current register value, right justified in the
    /// return value.
    virtual uint64_t read() const;

    /// Get the register value as a conversion (cast) using read().
    virtual operator uint64_t () const;


    ////////////////////////// Implementation ////////////////////////////

protected:

    /// The parent PoreInterface object
    PoreInterface* iv_interface;

    /// The PORE ISA encoding of register
    PoreRegisterEncoding iv_encoding;

    //////////////////////////    Safety      ////////////////////////////
    
private:

    PoreRegister(const PoreRegister& i_rhs);
    PoreRegister& operator=(PoreRegister& i_rhs);
};


////////////////////////////////////////////////////////////////////////////
// PoreRegisterWritable
////////////////////////////////////////////////////////////////////////////

/// A writable PoreRegister
///
/// This subclass extends the PoreRegister class by the addition of write
/// access and a write() method.

class 
vsbe::PoreRegisterWritable : public PoreRegister {

public:

    ////////////////////////////// Creators //////////////////////////////

    /// Create a PoreRegisterWritable
    ///
    /// \param[in] i_interface A pointer to the parent PoreInterface object that
    /// implements the underlying registerRead() and registerWrite() methods.
    ///
    /// \param[in] i_register A value from the enumeration
    /// PoreRegisterEncoding identifying the register.
    PoreRegisterWritable(PoreInterface* i_interface, 
                         const PoreRegisterEncoding i_register);

    virtual ~PoreRegisterWritable();


    //////////////////////////// Manipulators ////////////////////////////

    /// Implement the PoreRegister register write
    ///
    /// \param[in] i_data The data to be written to the register
    ///
    /// For registers smaller than 64 bits, the low-order bits of \a i_data
    /// are inserted into the hardware register.  Note that if the embedded
    /// call of PoreInterface::registerWritw() fails it is considered a bug in
    /// the model.
    ///
    /// \retval data This method returns its input \a data.
    virtual uint64_t write(const uint64_t i_data);

    /// Assign a value to a register using operator=().  See write().
    virtual uint64_t operator=(const uint64_t& i_data);


    //////////////////////////    Safety      ////////////////////////////
    
private:

    PoreRegisterWritable(const PoreRegisterWritable& i_rhs);
    PoreRegisterWritable& operator=(PoreRegisterWritable& i_rhs);
};


////////////////////////////////////////////////////////////////////////////
// PoreDataBuffer
////////////////////////////////////////////////////////////////////////////

/// A PoreRegister as an ecmdDataBuffer
///
/// This subclass extends the PoreRegisterWritable class by the addition of
/// selected methods of the ecmdDataBuffer, to provide a familiar and
/// convenient API for manipulating data in the PoreVe.  Only the 64-bit data
/// registers D0 and D1 are implemented using this class.  
///
/// Note that the eCmd-like methods are all implemented in functional form for
/// simplicty, and operate on uint64_t data values exclusively. There is no
/// error checking for bit positions, which are assumed to fall with the range
/// 0:63.

class 
vsbe::PoreDataBuffer : public PoreRegisterWritable {

public:

    ////////////////////////////// Creators //////////////////////////////

    /// Create a PoreDataBuffer
    ///
    /// \param[in] i_interface A pointer to the parent PoreInterface object that
    /// implements the underlying registerRead() and registerWrite() methods.
    ///
    /// \param[in] i_register A value from the enumeration
    /// PoreRegisterEncoding identifying the register.
    PoreDataBuffer(PoreInterface* i_interface,
                   const PoreRegisterEncoding i_register);

    virtual ~PoreDataBuffer();


    ////////////////////////////  Accessors   ////////////////////////////

    /// Return \a true if bit \a i_bit is set, otherwise \a false
    virtual bool isBitSet(const uint32_t i_bit);

    /// Return \a true if bit \a i_bit is clear, otherwise \a false
    virtual bool isBitClear(const uint32_t i_bit);

    /// Extract \a i_len bits from \a i_start and right justify into a uint64_t
    virtual uint64_t extractToRight(const uint32_t i_start,
                                    const uint32_t i_len);

    using PoreRegister::operator uint64_t;


    //////////////////////////// Manipulators ////////////////////////////

    /// Assign a value to a register using operator=().  See write().
    virtual uint64_t operator=(const uint64_t& i_data);

    /// Set bit \a i_bit and return the new 64-bit value
    virtual uint64_t setBit(const uint32_t i_bit);

    /// Clear bit \a i_bit and return the new 64-bit value
    virtual uint64_t clearBit(const uint32_t i_bit);

    /// Insert \a i_len low-order bits of \a i_data at \a i_start and return
    /// the new 64-bit value
    virtual uint64_t insertFromRight(const uint64_t i_data,
                                     const uint32_t i_start,
                                     const uint32_t i_len);


    //////////////////////////    Safety      ////////////////////////////
    
private:

    PoreDataBuffer(const PoreDataBuffer& i_rhs);
    PoreDataBuffer& operator=(PoreDataBuffer& i_rhs);
};


/// \bug These register layouts are defined in pore_model/ibuf/pore_regs.h,
/// however that header also includes a definition of a PoreAddress type that
/// conflicts with the type we define in PoreAddress.H.  If the conflict can
/// be removed from pore_regs.h then we can #include that file instead of
/// duplicating it here.

#include <endian.h>

typedef union {
        struct {
#if (__BYTE_ORDER == __BIG_ENDIAN)
                uint64_t i2c_engine_identifier : 4;
                uint64_t reserved0 : 1;
                uint64_t i2c_engine_address_range : 3;
                uint64_t reserved1 : 3;
                uint64_t i2c_engine_port : 5;
                uint64_t reserved2 : 1;
                uint64_t i2c_engine_device_id : 7;
                uint64_t reserved3 : 2;
                uint64_t i2c_engine_speed : 2;
                uint64_t i2c_done_max_polls : 4; /* I2C_E0_PARAM only */
                uint64_t reserved4 : 32;
#else
                uint64_t reserved4 : 32;
                uint64_t i2c_done_max_polls : 4; /* I2C_E0_PARAM only */
                uint64_t i2c_engine_speed : 2;
                uint64_t reserved3 : 2;
                uint64_t i2c_engine_device_id : 7;
                uint64_t reserved2 : 1;
                uint64_t i2c_engine_port : 5;
                uint64_t reserved1 : 3;
                uint64_t i2c_engine_address_range : 3;
                uint64_t reserved0 : 1;
                uint64_t i2c_engine_identifier : 4;
#endif
        };
        uint64_t val;
} pore_i2c_en_param_reg;


#endif  // __VSBE_POREREGISTER_H
