//
// Generated file, do not edit! Created by nedtool 5.3 from nodes/messages/UENotification.msg.
//

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#ifndef __UENOTIFICATION_M_H
#define __UENOTIFICATION_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0503
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Enum generated from <tt>nodes/messages/UENotification.msg:17</tt> by nedtool.
 * <pre>
 * enum NotificationType
 * {
 *     N_SCHEDULE = 0;
 *     N_TIMEOUT = 1;
 *     N_REQUERY = 2;
 *     N_RECACHE = 3;
 * }
 * </pre>
 */
enum NotificationType {
    N_SCHEDULE = 0,
    N_TIMEOUT = 1,
    N_REQUERY = 2,
    N_RECACHE = 3
};

/**
 * Class generated from <tt>nodes/messages/UENotification.msg:25</tt> by nedtool.
 * <pre>
 * message UENotification
 * {
 *     int fileId;
 *     int type \@enum(NotificationType);
 * }
 * </pre>
 */
class UENotification : public ::omnetpp::cMessage
{
  protected:
    int fileId;
    int type;

  private:
    void copy(const UENotification& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const UENotification&);

  public:
    UENotification(const char *name=nullptr, short kind=0);
    UENotification(const UENotification& other);
    virtual ~UENotification();
    UENotification& operator=(const UENotification& other);
    virtual UENotification *dup() const override {return new UENotification(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getFileId() const;
    virtual void setFileId(int fileId);
    virtual int getType() const;
    virtual void setType(int type);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const UENotification& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, UENotification& obj) {obj.parsimUnpack(b);}


#endif // ifndef __UENOTIFICATION_M_H

