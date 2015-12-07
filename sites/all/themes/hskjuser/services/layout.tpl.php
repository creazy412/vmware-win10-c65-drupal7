
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
  <head>
    <?php print $head; ?>
    <title><?php print $head_title; ?></title>
	<!-- META FOR IOS & HANDHELD -->
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=yes"/>
    <meta name="HandheldFriendly" content="true" />
    <meta name="apple-touch-fullscreen" content="YES" />
    <!-- //META FOR IOS & HANDHELD -->
    <?php print $styles; ?>
    <?php print $scripts; ?>
  </head>

  <body class="<?php print $classes; ?>"<?php print $attributes;?>>
  
  <div id="main-wrapper" class="wrapper">
        <!-- MAIN CONTENT -->
        <div id="main-content" class="<?php print $content_width;?> section">
          <div class="grid-inner clearfix">

            <?php print render($title_prefix); ?>
              <?php if ($node->title): ?>
                <h1 id="page-title"><?php print $node->title; ?></h1>
            <?php endif; ?>

            <?php print render($title_suffix); ?>

            <?php if ($action_links = render($action_links)): ?>
              <ul class="action-links"><?php print $action_links; ?></ul>
            <?php endif; ?>

<div id="article-<?php print $node->nid; ?>" class="article <?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php if ($title && !$page): ?>
    <div class="header article-header">
      <?php print render($title_prefix); ?>
      <?php if ($node->title): ?>
        <h2<?php print $title_attributes; ?>>
          <a href="<?php print $node_url; ?>"><?php print $node->title; ?></a>
        </h2>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
    </div>
  <?php endif; ?>

  <?php if ($display_submitted): ?>
    <div class="footer submitted">
      <?php print $user_picture; ?>
      <?php
        print t('Submitted by !username on !datetime', array(
          '!username' => $name,
          '!datetime' => '<span class="time pubdate" title="' . $datetime . '">' . $date . '</span>',
        ));
      ?>
    </div>
  <?php endif; ?>

  <div<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </div>

  <?php if ($links = render($content['links'])): ?>
    <div class="menu node-links clearfix"><?php print $links; ?></div>
  <?php endif; ?>

  <?php print render($content['comments']); ?>
</div>
          </div>
        </div>
        <!-- //MAIN CONTENT -->
  </body>
</html>



