jMailComposer 1.2
Copyright JavaZOOM 1999-2003
http://www.javazoom.net

==========================================================
jMailComposer support page :
http://www.javazoom.net/jzservlets/jmailcomposer/

Any question about jMailComposer ? Try our JSP Forums :
http://www.javazoom.net/services/forums/index.jsp
==========================================================

* Open documentation/index.html for instructions *


-----------------
09/01/2003 : v1.2
-----------------
- SendMailListener API improved :
  prepareEmail(), sendEmail(), emailSent() methods added.

> Multilanguage Pack add-on available
  - multilanguage/ template added.
  - English, Simplified Chinese, French support added.

+ ServletExec 4.1.1 support added.

-----------------
01/06/2003 : v1.1
-----------------
- Organization header field added.
- Return receipt feature added.
- Bug fix for multilanguage support.
- EAR archive available.

+ Add-ons section added :
  MultilanguagePack available.

+ Tomcat 4.1.18-LE support added.

-----------------
11/04/2002 : v1.0
-----------------
jMailComposer is a Servlet/JSP component that allows to send emails from web browser.
It supports to, replyto, cc, bcc email addresses and personal names. It provides message
priority and file attachments. jMailComposer JSP files could be customized to your own
web design and language. You could also include custom fields coming from HTML form into
email message.
This package includes 3 samples to let you customize jMailComposer as you need :
/blank/ is a "blank template" showing most possibilities of jMailComposer.
/contact/ is a "Technical support contact" sample. It includes day phone and product custom fields.
/resume/ is a "Send us your resume" sample. It shows how to add attachments too.

First release includes :
~~~~~~~~~~~~~~~~~~~~~~
* Sendmail features :
  - Multiple email addresses (to, replyto, cc, bcc) support.
  - Multiple matching personal names (to, replyto, cc, bcc) support.
  - Message subject and content.
  - Message priority (highest, high, normal, low, lowest) support.
  - Custom fields support (phone, name, customerid, ...)

* Multiple files attachement :
  - Upload progress bar.
  - Max. total size limit for upload store.
  - Blacklist/Whitelist.

* More :
  - Custom charset for multilanguage support.
  - No cookies support needed to run jMailComposer.

+ JRun 4.0 support added.
+ Resin 2.1.2 support added.
+ Weblogic 6.1 support added.
+ WebSphereAEs 4.0.3 support added.
+ Tomcat 4.1.12 support added.
+ Tomcat 4.0.4-LE support added.
+ Tomcat 4.0.5 support added.
+ Tomcat 3.3.1 support added.
+ Tomcat 3.2.4 support added.


Note :
----
jMailComposer includes JavaZOOM's UploadBean to support file uploading for attachments.
- UploadBean includes JavaMail 1.2 (+JAF 1.0.2) free implementation from SUN
  JavaMail homepage : http://java.sun.com/products/javamail/
- UploadBean includes free STRUTS multipart parser.
  UploadBean homepage : http://www.javazoom.net/jzservlets/uploadbean/uploadbean.html
  STRUTS homepage : http://jakarta.apache.org/struts/
