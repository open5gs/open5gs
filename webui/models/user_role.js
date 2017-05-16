module.exports = (sequelize, DataTypes) => {
  const UserRole = sequelize.define('UserRole', {
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
        UserRole.hasMany(models.User);
      }
    },
    freezeTableName: true
  });

  return UserRole;
};