module.exports = (sequelize, DataTypes) => {
  const AccountRole = sequelize.define('AccountRole', {
    role: {
      type: DataTypes.STRING,
      allowNull: false,
      validate: {
        isAlpha: true,
        notEmpty: true
      }
    }
  }, {
    classMethods: {
      associate: models => {
        AccountRole.Account = AccountRole.hasMany(models.Account);
      }
    },
    freezeTableName: true
  });

  return AccountRole;
};