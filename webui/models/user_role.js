module.exports = (sequelize, DataTypes) => {
  const UserRole = sequelize.define('UserRole', {
    role: {
      type: DataTypes.STRING,
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
    }
  });

  return UserRole;
};