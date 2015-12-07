<?php

/**
 * @file
 * The PHP page that serves all page requests on a Drupal installation.
 *
 * The routines here dispatch control to the appropriate handler, which then
 * prints the appropriate page.
 *
 * All Drupal code is released under the GNU General Public License.
 * See COPYRIGHT.txt and LICENSE.txt.
 */

/**
 * Root directory of Drupal installation.
 */
define('DRUPAL_ROOT', getcwd());

require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_DATABASE);

$result = db_query('select * from {node} where uid = :uid', array(':uid' => '29'));

foreach($result as $raw){
	echo $raw->title."(".$raw->nid.")"."<br/>";
}

echo "==================隔========================"."<br/>";

$query = db_select('role','r');
$query->condition('rid',7)
	->fields('r',array('name'));
$result = $query->execute();

foreach($result as $raw){
	echo $raw->name."<br/>";
}
echo "==================隔1======================="."<br/>";

$query = db_select('node','n');
$query
	->condition('type','general_task')
	->fields('n',array('title','nid'))
	->range(0,100);
$result = $query->execute();

foreach($result as $row)
{
	echo $row->title."(".$row->nid.")"."<br/>";
}
