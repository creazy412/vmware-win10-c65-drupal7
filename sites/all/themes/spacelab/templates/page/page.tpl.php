<?php

/**
 * @file
 * Kalatheme's implementation to display a single Drupal page.
 *
 * The doctype, html, head, and body tags are not in this template. Instead
 * they can be found in the html.tpl.php template normally located in the
 * core/modules/system directory.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/bartik.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or
 *   prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 * - $hide_site_name: TRUE if the site name has been toggled off on the theme
 *   settings page. If hidden, the "element-invisible" class is added to make
 *   the site name visually hidden, but still accessible.
 * - $hide_site_slogan: TRUE if the site slogan has been toggled off on the
 *   theme settings page. If hidden, the "element-invisible" class is added to
 *   make the site slogan visually hidden, but still accessible.
 *
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the
 *   site, if they have been configured.
 * - $main_menu_expanded (array): An array containing 2 depths of the Main
 *   menu links
 *   for the site, if they have been configured.
 * - $secondary_menu (array): An array containing the Secondary menu links for
 *   the site, if they have been configured.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title: The page title, for use in the actual HTML content.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 * - $messages: HTML for status and error messages. Should be displayed
 *   prominently.
 * - $tabs (array): Tabs linking to any sub-pages beneath the current page
 *   (e.g., the view and edit tabs when displaying a node).
 * - $action_links (array): Actions local to the page, such as 'Add menu' on
 *   the menu administration interface.
 * - $feed_icons: A string of all feed icons for the current page.
 * - $node: The node entity, if there is an automatically-loaded node
 *   associated with the page, and the node ID is the second argument
 *   in the page's path (e.g. node/12345 and node/12345/revisions, but not
 *   comment/reply/12345).
 *
 * Kalatheme:
 * - $no_panels: A boolean that is true if the current page is not a panels page
 * - $always_show_page_title: A boolean that is true if we're to always print
 *   the page title, even on panelized pages.
 *
 * Regions:
 * - $page['content']: The main content of the current page.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 * @see bartik_process_page()
 * @see html.tpl.php
 *
 * @ingroup themeable
 */
?>
<div id="page-wrapper"><div id="page">
	<?php if ($page['header_top_left'] || $page['header_top_right']) :?>
	<!-- #header-top -->
	<div id="header-top" class="clearfix">
		  <div class="container">

		      <!-- #header-top-inside -->
		      <div id="header-top-inside" class="clearfix">
		          <div class="row">
		          
		          <?php if ($page['header_top_left']) :?>
		          <div class="<?php print $header_top_left_grid_class; ?>">
		              <!-- #header-top-left -->
		              <div id="header-top-left" class="nav navbar-left clearfix">
		                  <?php print render($page['header_top_left']); ?>
		              </div>
		              <!-- EOF:#header-top-left -->
		          </div>
		          <?php endif; ?>
		          
		          <?php if ($page['header_top_right']) :?>
		          <div class="<?php print $header_top_right_grid_class; ?>">
		              <!-- #header-top-right -->
		              <div id="header-top-right" class="nav navbar-right clearfix">
		                  <?php print render($page['header_top_right']); ?>
		              </div>
		              <!-- EOF:#header-top-right -->
		          </div>
		          <?php endif; ?>
		          
		          </div>
		      </div>
		      <!-- EOF: #header-top-inside -->

		  </div>
	</div>
	<!-- EOF: #header-top -->    
	<?php endif; ?>

<header id="header" class="header" role="header">
  <div class="container">
    <nav class="navbar navbar-default" role="navigation">
      <!-- Brand and toggle get grouped for better mobile display -->
      <div class="navbar-header">
        <a class="navbar-toggle admin-menu" href="#mmenu_left" role="button">
          <span class="sr-only"><?php print t('Toggle navigation'); ?></span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
        </a>
        <a href="<?php print $front_page; ?>" id="logo" class="navbar-brand">
          <?php print $site_name; ?>
        </a>
      </div> <!-- /.navbar-header -->

      <!-- Collect the nav links, forms, and other content for toggling -->
      <div class="collapse navbar-collapse" id="navbar-collapse">
        <?php if ($logged_in && $main_menu) :?>
	        <ul id="main-menu" class="menu nav navbar-nav">
            <?php print render($main_menu); ?>
         </ul>
        <?php endif; ?>
        <?php if ($logged_in && $search_form): ?>
          <?php print $search_form; ?>
        <?php endif; ?>
      </div><!-- /.navbar-collapse -->
    </nav><!-- /.navbar -->
  </div> <!-- /.container -->
</header>


	<?php if ($page['banner']) : ?>
	<!-- #banner -->
	<div id="banner" class="clearfix">
		  <div class="container">
		      
		      <!-- #banner-inside -->
		      <div id="banner-inside" class="clearfix">
		          <div class="row">
		              <div class="col-md-12">
		              <?php print render($page['banner']); ?>
		              </div>
		          </div>
		      </div>
		      <!-- EOF: #banner-inside -->        

		  </div>
	</div>
	<!-- EOF:#banner -->
	<?php endif; ?>

  <!-- Page Main -->
  <div id="main-wrapper" class="clearfix">
  	<div id="main" class="main">
    	<div class="container">

				<?php if ($page['sidebar_first']):?>
				<aside class="<?php print $sidebar_grid_class; ?>">  
				    <!--#sidebar-first-->
				    <section id="sidebar-first" class="sidebar clearfix">
				    <?php print render($page['sidebar_first']); ?>
				    </section>
				    <!--EOF:#sidebar-first-->
				</aside>
				<?php endif; ?>

				<?php if ($no_panels): ?>
					<section class="<?php print $main_grid_class; ?>">
				<?php else: ?>
				  <section class="row">
				<?php endif; ?>
				  <div id="top-content">
				    <div class="column">
				      <a id="main-content"></a>
				      <?php if (($no_panels || $always_show_page_title) && $title): ?>
				        <h1 id="page-title" class="title">
				          <?php print $title; ?>
				        </h1>
				      <?php endif; ?>

				      <?php if ($breadcrumb): ?>
				        <div id="breadcrumb" class="visible-desktop">
				          <?php print $breadcrumb; ?>
				        </div>
				      <?php endif; ?>
				      <?php if ($messages): ?>
				        <div id="messages">
				          <?php print $messages; ?>
				        </div>
				      <?php endif; ?>

				      <?php if (!empty($tabs['#primary']) || !empty($tabs['#secondary'])): ?>
				        <div id="tabs">
				          <?php print render($tabs); ?>
				        </div>
				      <?php endif; ?>

				      <?php if ($action_links): ?>
				        <div id="action-links">
				          <?php print render($action_links); ?>
				        </div>
				      <?php endif; ?>
				    </div>
				  </div> <!-- /.section, /#top-content -->
				  <div id="content">
				    <div class="column">
				      <?php print render($page['content']); ?>
				    </div>
				  </div> <!-- /.section, /#content -->
				</section>

		    <?php if ($page['sidebar_second']):?>
		    <aside class="<?php print $sidebar_grid_class; ?>">
		        <!--#sidebar-second-->
		        <section id="sidebar-second" class="sidebar clearfix">
		        <?php print render($page['sidebar_second']); ?>
		        </section>
		        <!--EOF:#sidebar-second-->
		    </aside>
		    <?php endif; ?>
      </div>

		</div><!-- /#main -->

		  <?php if ($page['bottom_content']):?>
		  <!-- #bottom-content -->
		  <div id="bottom-content" class="clearfix">
		      <div class="container">

		          <!-- #bottom-content-inside -->
		          <div id="bottom-content-inside" class="clearfix">
		              <div class="row">
		                  <?php print render($page['bottom_content']); ?>
		              </div>
		          </div>
		          <!-- EOF:#bottom-content-inside -->

		      </div>
		  </div>
		  <!-- EOF: #bottom-content -->
		  <?php endif; ?>
  </div> <!-- /#main-wrapper -->

<?php if ($page['footer_first'] || $page['footer_second'] || $page['footer_third'] || $page['footer_fourth'] || $page['footer_fifth']|| $page['footer_sixth']):?>
<!-- #footer -->
<footer id="footer" class="clearfix">
    <div class="container">
    
        <!-- #footer-inside -->
        <div id="footer-inside" class="clearfix">
            <div class="row">
                <div class="col-md-2">
                    <?php if ($page['footer_first']):?>
                    <div class="footer-area">
                    <?php print render($page['footer_first']); ?>
                    </div>
                    <?php endif; ?>
                </div>
                
                <div class="col-md-2">
                    <?php if ($page['footer_second']):?>
                    <div class="footer-area">
                    <?php print render($page['footer_second']); ?>
                    </div>
                    <?php endif; ?>
                </div>

                <div class="col-md-2">
                    <?php if ($page['footer_third']):?>
                    <div class="footer-area">
                    <?php print render($page['footer_third']); ?>
                    </div>
                    <?php endif; ?>
                </div>

                <div class="col-md-2">
                    <?php if ($page['footer_fourth']):?>
                    <div class="footer-area">
                    <?php print render($page['footer_fourth']); ?>
                    </div>
                    <?php endif; ?>
                </div>

                <div class="col-md-2">
                    <?php if ($page['footer_fifth']):?>
                    <div class="footer-area">
                    <?php print render($page['footer_fifth']); ?>
                    </div>
                    <?php endif; ?>
                </div>
                <div class="col-md-2">
                    <?php if ($page['footer_sixth']):?>
                    <div class="footer-area">
                    <?php print render($page['footer_sixth']); ?>
                    </div>
                    <?php endif; ?>
                </div>
            </div>
        </div>
        <!-- EOF: #footer-inside -->
    
    </div>
</footer> 
<!-- EOF #footer -->
<?php endif; ?>

<footer id="subfooter" class="clearfix">
    <div class="container">
      <?php if ($page['footer']):?>
        <!-- #subfooter-inside -->
        <div id="subfooter-inside" class="clearfix">
            <div class="row">
                <div class="col-md-12">
		                  <?php print render($page['footer']); ?>
                </div>
            </div>
        </div>
        <!-- EOF: #subfooter-inside -->
      <?php endif; ?>   
    </div>
</footer>
<!-- EOF:#subfooter -->

</div></div> <!-- /#page, /#page-wrapper -->
