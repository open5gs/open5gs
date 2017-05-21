module.exports = (sequelize, DataTypes) => {
  const Account = sequelize.define('Account', {
    username: {
      type: DataTypes.STRING,
      allowNull: false,
      validate: {
        isAlpha: true,
        notEmpty: true
      }
    },
    password: {
      type: DataTypes.STRING,
      allowNull: false
    }
  }, {
    classMethods: {
      associate: models => {
        Account.AccountRole = Account.belongsTo(models.AccountRole, {
          onDelete: 'CASCADE',
          foreignKey: {
            allowNull: false
          }
        });
      }
    },
    freezeTableName: true
  });

  return Account;
};