Adds a new resource to get HTML content for a given node.

Requests details:

[endpoint]/html_content/layout
arguments:
    - node: node id to get content for
    - username: drupal username, logs this user in to get content based
      on this user being the logged in user.

This request will look for template files in your theme, they should be
stored in a folder called 'services'. The overall page layout template
should be called 'layout.tpl.php', within this template you will be
responsible for returning the correct content for the node being requested.

[endpoint]/html_content/block
    - node: node id to get content for
    - element: the block delta value, or custom id*
    - username: drupal username, logs this user in to get content based
      on this user being the logged in user.

* To return content for a custom block (i.e block content that does not
exist in the block table) your theme needs to implement a function with
a name in the following format:
    - [theme_key]_services_block_[element]
      - the node id will be passed as an argument to the function.
