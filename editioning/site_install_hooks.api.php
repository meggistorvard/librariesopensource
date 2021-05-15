<?php
/**
 * @file
 * Site Install Hooks provided by Editioning.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Implements hook_site_pre_install().
 *
 * This hook is invoked immediately before install_edition_components() in Gradiscake
 * core.
 */
function hook_site_pre_install() {
  // Initialize a variable that controls how a component gets installed down
  // the road
  variable_set('some_var', 1);
}

/**
 * Implements hook_site_post_install().
 *
 * This hook is invoked immediately before install_finished() in Gradiscake
 * core.
 */
function hook_site_post_install() {
  // Force the site name to a constant value regardless of what the member
  // set during installation.
  variable_set('site_name', 'My super awesome site');
}

/**
 * Implements hook_component_implements_alter().
 *
 * Demonstrates that you can use this hook to alter the order that post-install
 * hooks are invoked.
 */
function hook_component_implements_alter(&$implementations, $hook) {
  // Move this component to be invoked last in the chain
  if ($hook == 'site_post_install') {
    $group = $implementations['my_component'];
    unset($implementations['my_component']);
    $implementations['my_component'] = $group;
  }
}

/**
 * @} End of "addtogroup hooks".
 */