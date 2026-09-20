#include <fmt/format.h>
#include <odycompiler/parser/testparser/expression/expression_parser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser::testparser::expression {
    std::unique_ptr<odyplan::v3::basic::expression::logical::IExpression> ExpressionParser::parseExpression(const nlohmann::json &expressionJson) {
        try {
            switch (const auto expressionType = parseExpressionType(expressionJson)) {
                case odyplan::v3::basic::expression::logical::ExpressionType::COMPARE: {
                    const auto opr = parseCompareOperator(expressionJson);

                    auto left = parseExpression(expressionJson.value("left", nlohmann::json::object()));
                    auto right = parseExpression(expressionJson.value("right", nlohmann::json::object()));

                    SPDLOG_DEBUG("Creating CompareExpression with op={]", std::to_underlying(opr));

                    return std::make_unique<odyplan::v3::basic::expression::logical::CompareExpression>(std::move(left), opr, std::move(right));
                }
                case odyplan::v3::basic::expression::logical::ExpressionType::LOGICAL: {
                    if (const auto opr = parseLogicalOperator(expressionJson); opr == odyplan::v3::basic::expression::LogicalOp::NOT) {
                        auto left = parseExpression(expressionJson.value("left", nlohmann::json::object()));

                        SPDLOG_DEBUG("Creating LogicalExpression with op={]", std::to_underlying(opr));

                        return std::make_unique<odyplan::v3::basic::expression::logical::LogicalExpression>(opr, std::move(left));
                    } else {
                        auto left = parseExpression(expressionJson.value("left", nlohmann::json::object()));
                        auto right = parseExpression(expressionJson.value("right", nlohmann::json::object()));

                        SPDLOG_DEBUG("Creating LogicalExpression with op={]", std::to_underlying(opr));

                        return std::make_unique<odyplan::v3::basic::expression::logical::LogicalExpression>(std::move(left), opr, std::move(right));
                    }
                }
                case odyplan::v3::basic::expression::logical::ExpressionType::FIELD_REFERENCE: {
                    const std::string field = expressionJson.value("field", "");
                    const int portIndex = expressionJson["port"];

                    if (field.empty()) {
                        throw std::runtime_error("FieldReferenceExpression must have 'field' name");
                    }

                    SPDLOG_DEBUG("Creating FieldReferenceExpression with field={}, port={}", field, portIndex);

                    return std::make_unique<odyplan::v3::basic::expression::logical::FieldReferenceExpression>(field, portIndex);
                }
                case odyplan::v3::basic::expression::logical::ExpressionType::CONSTANT: {
                    const std::string value = expressionJson.value("value", "");
                    const int typeId = expressionJson["typeId"];

                    if (value.empty()) {
                        throw std::runtime_error("FieldReferenceExpression must have 'value'");
                    }

                    SPDLOG_DEBUG("Creating ConstantExpression with value={}, typeId={}", value, typeId);

                    return std::make_unique<odyplan::v3::basic::expression::logical::ConstantExpression>(typeId, value);
                }
                default: {
                    throw std::runtime_error(fmt::format("Unknown ExpressionType: {}", static_cast<int>(expressionType)));
                }
            }
        } catch (std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse Expression: {}", e.what()));
        }
    }

    odyplan::v3::basic::expression::logical::ExpressionType ExpressionParser::parseExpressionType(const nlohmann::json &expressionJson) {
        try {
            if (!expressionJson.contains("type")) {
                throw std::runtime_error("Expression must contain 'type' field (int)");
            }

            if (const auto& typeJson = expressionJson["type"]; typeJson.is_number()) {
                const int value = typeJson.get<int>();
                return static_cast<odyplan::v3::basic::expression::logical::ExpressionType>(value);
            }

            throw std::runtime_error("Expressions 'type' field of unsupported type");
        } catch (std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse Expression: {}", e.what()));
        }
    }

    odyplan::v3::basic::expression::CompareOp ExpressionParser::parseCompareOperator(const nlohmann::json &expressionJson) {
        try {
            if (!expressionJson.contains("op")) {
                throw std::runtime_error("CompareExpression must contain 'op' field (int)");
            }

            if (const auto& opJson = expressionJson["op"]; opJson.is_number()) {
                const int value = opJson.get<int>();
                return static_cast<odyplan::v3::basic::expression::CompareOp>(value);
            }

            throw std::runtime_error("CompareExpressions 'type' field of unsupported type");
        } catch (std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse CompareExpression: {}", e.what()));
        }
    }

    odyplan::v3::basic::expression::LogicalOp ExpressionParser::parseLogicalOperator(const nlohmann::json &expressionJson) {
        try {
            if (!expressionJson.contains("op")) {
                throw std::runtime_error("LogicalExpression must contain 'op' field (int)");
            }

            if (const auto& opJson = expressionJson["op"]; opJson.is_number()) {
                const int value = opJson.get<int>();
                return static_cast<odyplan::v3::basic::expression::LogicalOp>(value);
            }

            throw std::runtime_error("LogicalExpression 'type' field of unsupported type");
        } catch (std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse LogicalExpression: {}", e.what()));
        }
    }
}
