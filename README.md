IRC Rules and Documentation

Clients

   A client is anything connecting to a server that is not another
   server.  Each client is distinguished from other clients by a unique
   nickname having a maximum length of nine (9) characters.  See the
   protocol grammar rules for what may and may not be used in a
   nickname.  In addition to the nickname, all servers must have the
   following information about all clients: the real name of the host
   that the client is running on, the username of the client on that
   host, and the server to which the client is connected.


Operators

   To allow a reasonable amount of order to be kept within the IRC
   network, a special class of clients (operators) is allowed to perform
   general maintenance functions on the network.  Although the powers
   granted to an operator can be considered as 'dangerous', they are
   nonetheless required.  Operators should be able to perform basic
   network tasks such as disconnecting and reconnecting servers as
   needed to prevent long-term use of bad network routing.  In
   recognition of this need, the protocol discussed herein provides for
   operators only to be able to perform such functions.  See sections
   4.1.7 (SQUIT) and 4.3.5 (CONNECT).

   A more controversial power of operators is the ability  to  remove  a
   user  from  the connected network by 'force', i.e. operators are able
   to close the connection between any client and server.   The
   justification for  this  is delicate since its abuse is both
   destructive and annoying.  For further details on this type of
   action, see section 4.6.1 (KILL).


Channels

   A channel is a named group of one or more clients which will all
   receive messages addressed to that channel.  The channel is created
   implicitly when the first client joins it, and the channel ceases to
   exist when the last client leaves it.  While channel exists, any
   client can reference the channel using the name of the channel.

   Channels names are strings (beginning with a '&' or '#' character) of
   length up to 200 characters.  Apart from the the requirement that the
   first character being either '&' or '#'; the only restriction on a
   channel name is that it may not contain any spaces (' '), a control G
   (^G or ASCII 7), or a comma (',' which is used as a list item
   separator by the protocol).

   There are two types of channels allowed by this protocol.  One is a
   distributed channel which is known to all the servers that are
   connected to the network. These channels are marked by the first
   character being a only clients on the server where it exists may join
   it.  These are distinguished by a leading '&' character.  On top of
   these two types, there are the various channel modes available to
   alter the characteristics of individual channels.  See section 4.2.3
   (MODE command) for more details on this.

   To create a new channel or become part of an existing channel, a user
   is required to JOIN the channel.  If the channel doesn't exist prior
   to joining, the channel is created and the creating user becomes a
   channel operator.  If the channel already exists, whether or not your
   request to JOIN that channel is honoured depends on the current modes
   of the channel. For example, if the channel is invite-only, (+i),
   then you may only join if invited.  As part of the protocol, a user
   may be a part of several channels at once, but a limit of ten (10)
   channels is recommended as being ample for both experienced and
   novice users.  See section 8.13 for more information on this.

   If the IRC network becomes disjoint because of a split between two
   servers, the channel on each side is only composed of those clients
   which are connected to servers on the respective sides of the split,
   possibly ceasing to exist on one side of the split.  When the split
   is healed, the connecting servers announce to each other who they
   think is in each channel and the mode of that channel.  If the
   channel exists on both sides, the JOINs and MODEs are interpreted in
   an inclusive manner so that both sides of the new connection will
   agree about which clients are in the channel and what modes the
   channel has.


Channel Operators

   The channel operator (also referred to as a "chop" or "chanop") on a
   given channel is considered to 'own' that channel.  In recognition of
   this status, channel operators are endowed with certain powers which
   enable them to keep control and some sort of sanity in their channel.
   As an owner of a channel, a channel operator is not required to have
   reasons for their actions, although if their actions are generally
   antisocial or otherwise abusive, it might be reasonable to ask an IRC
   operator to intervene, or for the usersjust leave and go elsewhere
   and form their own channel.

   The commands which may only be used by channel operators are:

        KICK    - Eject a client from the channel
        MODE    - Change the channel's mode
        INVITE  - Invite a client to an invite-only channel (mode +i)
        TOPIC   - Change the channel topic in a mode +t channel
   A channel operator is identified by the '@' symbol next to their
   nickname whenever it is associated with a channel (ie replies to the
   NAMES, WHO and WHOIS commands).


Connection 'Liveness'

   To detect when a connection has died or become unresponsive, the
   server must ping each of its connections that it doesn't get a
   response from in a given amount of time.

   If a connection doesn't respond in time, its connection is closed
   using the appropriate procedures.  A connection is also dropped if
   its sendq grows beyond the maximum allowed, because it is better to
   close a slow connection than have a server process block.


Establishing a server to client connection

   Upon connecting to an IRC server, a client is sent the MOTD (if
   present) as well as the current user/server count (as per the LUSER
   command).  The server is also required to give an unambiguous message
   to the client which states its name and version as well as any other
   introductory messages which may be deemed appropriate.

   After dealing with this, the server must then send out the new user's
   nickname and other information as supplied by itself (USER command)
   and as the server could discover (from DNS/authentication servers).
   The server must send this information out with NICK first followed by
   USER.


   SOURCE: https://www.rfc-editor.org/rfc/rfc1459#section-4.5.1   https://ircv3.net/irc/
   https://docs.kde.org/trunk5/en/konversation/konversation/irc-cmds.html
https://defs.ircdocs.horse/defs/numerics.html
https://dd.ircdocs.horse/refs/numerics/001.html