the creator of the channel should be operator? now it is

a channe named just # can't be created

If a client tries to change their nickname to the same nickname they already have, the IRC server should not perform any broadcast or state change

channels names case sensitive?? now yes

TO DO nick name sensitive?

TO DO crash with an ssl connection??

DONE commands case insensitive

TO DO LIST command

TO DO bot

DONE handle cap

DONE chiusura errore dopo cambiamente ERRNo
Interrupt signal received. Shutting down server...
Poll failed (error cause unavailable)
Server destroyed.

DONE able the quit also if not registered

DONE add a kick reason

DONE restriction on topic

DONE user number limit doesn't works

DONE: if a client is invited and the channel is key protected, should the invted still insert the key? now yes

DONE: can you kick yourself? or remove operator to yourself? now yes

!!!!!Error numerics like 401 ERR_NOSUCHNICK!!!!!

-----------------------------------------------------------------------------------------------------------
WHY DOES NICK CHANGE MESSAGE DOESN'T APPEAR ON PRIVATE CHATS' TAB IN HEXCHAT BUT APPEARS IN CHANNEL TABS?

and QUIT message is shown in both?


The reason HexChat displays NICK change messages in channel tabs but not in private chat tabs has to do with the nature of IRC (Internet Relay Chat) and how HexChat is designed to handle different types of events.

NICK Change: A NICK change is a network-wide event. When a user changes their nickname, the IRC server broadcasts this change to all channels that the user is in. HexChat, like most IRC clients, receives this message and displays it in the channel tabs to inform all users that someone has changed their name. However, a private chat (a "query" or "dialog" in IRC terms) is a direct one-on-one conversation. The private chat tab is specifically for the conversation with that one user. A NICK change message is not a direct message to you, so it's not relevant to the private chat tab's primary function, which is to display the chat history between you and that user.

QUIT Message: A QUIT message, on the other hand, is a server-wide event that is also directly relevant to any ongoing conversation. When a user quits the server, it means they are no longer online. This is a critical piece of information for all tabs where that user is present, including both channel tabs and private chat tabs. In a private chat, the QUIT message is a signal that the person has gone offline, and you can no longer send them messages directly until they reconnect. Therefore, HexChat displays this message in the private chat tab to notify you that the conversation has ended (at least for the time being).

In short, HexChat's behavior is based on the logical relevance of the IRC event to the specific tab. NICK changes are relevant to the channel-wide roster, while QUIT messages are relevant to the ability to communicate with the user, whether in a channel or a private chat

-----------------------------------------------------------------------------------------------------------
WHY HEXCHAT, WHEN I SEND A MESSAGE TO A CLIENT PRIVATLY DON'T ADD THE CHAT TO THE SIDEBAR IN MY WINDOW?

it's added just when the other client reply


This behavior is by design in HexChat and other traditional IRC clients. It's tied to the fundamental difference between a message you send and a message you receive.

When you send a private message to another client, it is a command you are giving to your client to send to the server. The client logs this message in the current window (e.g., the channel you're in or the server tab), but it doesn't automatically open a new private chat tab. This is because, from a client's perspective, the conversation hasn't officially started. You've initiated it, but you haven't received confirmation that the other user is even present or has acknowledged your message.

The new private chat tab is created when you receive a private message from another user. This is an incoming event that the client needs to highlight and make easy for you to respond to. The new tab serves as a dedicated space for this new or re-initiated conversation.

This design choice has a few practical reasons:

Preventing "Clutter": If a new tab were created every time you sent a private message, you could easily clutter your tab bar by sending messages to multiple people without them ever responding. This behavior keeps the interface clean and only shows you the conversations that are actually active.

Awaiting a "Query": In IRC, a private chat is often referred to as a "query." The term implies a question or an inquiry. You are "querying" the other user. The new tab only opens when the other user "queries" you back.

User Experience: The primary purpose of the private chat tab is to show you the messages you've received so you don't miss them. By only opening the tab when a message is received, HexChat is prioritizing the most critical information—messages from others that require your attention.

While this is the default behavior, some IRC clients and even HexChat scripts might offer an option to change it. However, the standard IRC client design philosophy is to manage the user's view based on incoming, actionable events.