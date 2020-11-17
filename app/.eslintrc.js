module.exports = {
  root: true,
  env: {
    browser: true,
    node: true,
  },
  parserOptions: {
    parser: 'babel-eslint',
  },
  extends: 'vuetify',
  // add your custom rules here
  rules: {
  },
  globals: {
    __BUILD_TIME__: 'readonly',
    __COMMIT_HASH__: 'readonly',
  }
}
