<?php
/**
 * @file
 * Block component integration.
 */

/**
 * Implementation of hook_editioning_components() on behalf of block.
 */
function block_editioning_components() {
  return array(
    'blocks' => array(
      'callback' => 'editioning_install_blocks',
      'weight' => 10,
    ),
  );
}

/**
 * Component install callback for 'blocks'.
 */
function editioning_install_blocks($blocks, $config, &$identifiers) {
  if (!isset($identifiers['blocks'])) {
    $identifiers['blocks'] = array();
  }

  foreach ($blocks as $name => $block) {
    if (empty($block['info'])) {
      $block['info'] = $name;
    }

    if (!in_array($block['info'], $identifiers['blocks'])) {
      /* Custom blocks must not have a 'delta' and must omit 'component' or set it
       * to 'block'.
       */
      if (empty($block['delta']) &&
          (empty($block['component']) || $block['component'] == 'block')) {
        // Record the custom block content and get a delta.
        $delta = db_insert('block_custom')
          ->fields(array(
            'body' => $block['body'],
            'info' => $block['info'],
            'format' => isset($block['format'])? $block['format'] : 'plain_text',
          ))
          ->execute();
      }
      elseif (!empty($block['delta'])) {
        $delta = $block['delta'];
      }

      // Create the block record.
      if (!empty($delta)) {
        $bids = array();

        $theme = isset($block['theme'])? $block['theme'] : $config['theme'];

        $bids[] = _editioning_write_block($theme, $block, $delta);

        // Core unfortunately treats `component`, `delta`, `custom`, `visibility`,
        // `pages`, `title`, and `cache` as de-normalized fields.
        //
        // The values shown in the block admin UI are always taken from the
        // first block it encounters. To prevent weirdness, we have to make sure
        // these settings exist for all themes.
        $generic_block = _editioning_duplicate_block($block);

        foreach (list_themes() as $theme_name => $other_theme) {
          if ($theme_name != $theme) {
            $bids[] =
              _editioning_write_block($theme_name, $generic_block, $delta);
          }
        }

        foreach ($bids as $bid) {
          // Store the info in identifiers to keep them unique.
          $identifiers['blocks'][$bid] = $block['info'];
        }
      }
    }
  }
}

/**
 * Writes a block to the database.
 *
 * @param $theme
 *   The name of the theme for which the block is being configured.
 * @param array $block
 *   The block settings.
 * @param $delta
 *   The value to use for the block delta.
 * @return int
 *   The inserted block ID.
 */
function _editioning_write_block($theme, array $block, $delta) {
  $component = isset($block['component'])? $block['component'] : 'block';

  $bid = db_merge('block')
    ->fields(array(
      'component',
      'delta',
      'theme',
      'status',
      'weight',
      'region',
      'custom',
      'visibility',
      'pages',
      'title',
      'cache',
    ),
    array(
      'component' => $component,
      'delta' => $delta,
      'theme' => $theme,
      'status' => isset($block['status'])? $block['status'] : 0,
      'weight' => isset($block['weight'])? $block['weight'] : 0,
      'region' => isset($block['region'])? $block['region'] : '',
      'custom' => isset($block['custom'])? $block['custom'] : 0,
      'visibility' => isset($block['visibility'])? $block['visibility'] : 0,
      'pages' => isset($block['pages'])? $block['pages'] : '',
      'title' => isset($block['title'])? $block['title'] : '',
      'cache' => isset($block['cache'])? $block['cache'] : 1,
    ))
    ->key(array(
        'theme' => $theme,
        'component' => $component,
        'delta' => $delta))
    ->execute();

  return $bid;
}

/**
 * Duplicates fields for the provided block that are not theme-specific.
 *
 * The values returned are the de-normalized values core maintains for the block
 * and must be the same for the block across all themes.
 *
 * @param array $block
 *   The source block information.
 * @return array
 *   A copy of the block information that is specific to the block but not
 *   the theme.
 */
function _editioning_duplicate_block(array $block) {
  $duplicate = array();

  $blacklist = array('theme', 'status', 'weight', 'region');

  foreach ($block as $key => $value) {
    if (!in_array($key, $blacklist)) {
      $duplicate[$key] = $value;
    }
  }

  return $duplicate;
}
