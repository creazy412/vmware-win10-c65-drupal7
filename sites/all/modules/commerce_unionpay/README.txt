INTRODUCTION
------------

The Commerce unionpay module implements [Unionpay](http://www.unionpay.com)
payment services for use with [Drupal Commerce](http://drupal.org/project/commerce).

REQUIREMENTS
------------

This module requires the following modules:
* Commerce (https://drupal.org/project/commerce)

CONFIGURATION
-------------

After successful installation, browse to the "Payment Methods" management page
under: Home » Administration » Store » Configuration » Payment methods
Path: admin/commerce/config/payment-methods or use the "Configure" link
displayed on the Modules management page.

Enable the Unionpay payment method, as described in the Drupal Commerce Payments
User Guide at: http://www.drupalcommerce.org/user-guide/payments
Follow all other setps as described in the Payments User Guide, edit the
Unionpay payment method (Rule) and then edit the Action "Enable payment method:
Unionpay".

TESTING
----------------
This is the test account provided by unionpay:
merId: 105550149170027
merAbbr: 商户名称
security_key: 88888888

Test card:
6212341111111111111 password: 111111 month: 12 CVN2: 123

Useful resources
----------------
For any question and problems, you may find some help on the Unionpay official site.

1 - Unionpay user helper:
https://online.unionpay.com/mer/pages/merser/helper.jsp
2 - Documentations download:
https://online.unionpay.com/mer/doc/viewDoc.action

Bugs/Features/Patches
---------------------
If you would like to report bugs, feature requests, or submit a patch, please
do so in the project's issue tracker at:
https://drupal.org/project/issues/2164979
